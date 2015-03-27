/*
Brett Binnersley, V00776751
Csc 360, Assignment #3
Part 3

March, 2015
*/

#include "shared.h"

//Forward Decl so that we can program main first
void TryExtractFile(FILE* file, char* extractName); //Try to extract a file. Print message if not found
void ExtractFile(FILE* file, char* name, int location); //Actually extract the file

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    printf("diskinfo <FILESYSTEM> <FILENAME>\n");
    return 0;
  }
  char* fileName = argv[1];
  char* extractFile = argv[2];

  FILE* file = 0;
  file = fopen(fileName, "r");
  if(file == 0)
  {
    printf("Failed to open the image file - Perhaps file not found.\n");
    return 0;
  }

  extractFile = covertToUpper(extractFile);
  TryExtractFile(file, extractFile);
  free(extractFile); //Note: convert to upper will return a new string, so we need to free it

  fclose(file);
  return 0;
}

//Try to extract a file from the filesystem (if it exists)
void TryExtractFile(FILE* file, char* extractName)
{
  int base = (SECTOR_ROOT_FIRST * SECTOR_SIZE);
  int cur = base;
  int numFiles = 0;
  fseek(file, base, SEEK_SET);
  int dirStatus = 0;
  fread(&dirStatus, 1, 1, file);
  while(dirStatus != DIR_FREE)
  {
    if (dirStatus != DIR_EMPTY)
    {
      int attributes;
      fseek(file, cur + DIR_ATTRIBUTES_POS, SEEK_SET);
      fread(&attributes, 1, DIR_ATTRIBUTES_SIZE, file);

      //REMOVE && !(attributes & DIR_VOLUME_LABEL) <**IF**> I want to include volumes in the file
      if(attributes != DIR_LONG_FILENAME && !(attributes & DIR_VOLUME_LABEL))
      {

        //Read in the filename
        fseek(file, cur + DIR_FILENAME_POS, SEEK_SET);
        char* filename = malloc(MAX_BUFFER_SIZE);
        memset(filename, '\0', MAX_BUFFER_SIZE);
        fread(filename, 1, DIR_FILENAME_SIZE, file);

        //Read in the extension
        fseek(file, cur + DIR_EXTENSION_POS, SEEK_SET);
        char* extension = malloc(MAX_BUFFER_SIZE);
        memset(extension, '\0', MAX_BUFFER_SIZE);
        fread(extension, 1, DIR_EXTENSION_SIZE, file);

        //Combine filename (and extension if it exists)
        char inFSname[MAX_BUFFER_SIZE];
        memset(&inFSname, '\0', MAX_BUFFER_SIZE);
        if(strlen(trimWhitespace(extension)))
        {
          sprintf (inFSname, "%s.%s", trimWhitespace(filename), trimWhitespace(extension));
        }
        else
        {
          sprintf (inFSname, "%s", trimWhitespace(filename));
        }

        //if strings match, we can extract this file from the FS now.
        if(strcmp(inFSname, extractName) == 0)
        {
          ExtractFile(file, extractName, cur);
          //Free memory then return. Normal clean up will not get called.
          free(extension);
          free(filename);
          return;
        }

        //Clean up memory
        free(extension);
        free(filename);
      }
    }
    cur = cur + DIR_OFFSET_SIZE;
    fseek(file, cur, SEEK_SET);
    fread(&dirStatus, 1, 1, file);
  }
  printf("File Not found\n");
}

//File exists inside the filesystem, extract it here
void ExtractFile(FILE* file, char* name, int clusterLoc)
{
  FILE* out = fopen(name, "w");
  int fileSize = getSizeFile(file, clusterLoc + DIR_FILE_SIZE_POS);


  fseek(file, clusterLoc + DIR_FIRST_LOG_CLUS_POS, SEEK_SET);
  int byte1 = 0;
  int byte2 = 0;
  fread(&byte1, 1, 1, file);
  fread(&byte2, 1, 1, file);
  int logicalCluster = (byte1 << 0) + (byte2 << 8);
  int size = 0;
  while(logicalCluster > FAT_LAST_CLUSTER_START)
  {
    int sectorPos = getSectorPos(logicalCluster);
    int readSize = min(fileSize - size, SECTOR_SIZE);

    //Read in 512 bytes
    char buffer[MAX_BUFFER_SIZE];
    fseek(file, sectorPos, SEEK_SET);
    fread(&buffer, 1, readSize, file);

    //Print them out to the file
    fwrite(buffer, sizeof(char), readSize, out);

    //Find next value in FAT table
    logicalCluster = getNextLogClusFromFAT(file, logicalCluster);
    size += SECTOR_SIZE;
  }

  printf("Successfully extracted %s\n", name);
}

//EOF

