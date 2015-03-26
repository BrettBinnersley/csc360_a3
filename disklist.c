/*
Brett Binnersley, V00776751
Csc 360, Assignment #3
Part 2

March, 2015
*/

#include "shared.h"

void ListFilesInRoot(); //Forward Decl so that we can program main first (cleaner code)

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Incorrect argument count\n");
    return 0;
  }
  char* fileName = argv[1];

  FILE* file = 0;
  file = fopen(fileName, "r");
  if(file == 0)
  {
    printf("Fail to open the image file.\n");
    return 0;
  }

  ListFilesInRoot(file);

  fclose(file);
  return 0;
}

//This function shares functionality with getNumFilesInRoot(FILE* )
void ListFilesInRoot(FILE* file)
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
        char type = 'F';
        if(attributes & DIR_SUBDIRECTORY)
        {
          type = 'D';
        }
        //Read in 4 bytes (little endian - turn it into a big endian int)
        fseek(file, cur + DIR_FILE_SIZE_POS, SEEK_SET);
        int byte1 = 0;
        int byte2 = 0;
        int byte3 = 0;
        int byte4 = 0;
        fread(&byte1, 1, 1, file);
        fread(&byte2, 1, 1, file);
        fread(&byte3, 1, 1, file);
        fread(&byte4, 1, 1, file);
        int size = (byte1 << 0) + (byte2 << 8) + (byte3 << 16) + (byte4 << 24);

        //Read in the filename
        fseek(file, cur + DIR_FILENAME_POS, SEEK_SET);
        char* filename = malloc(MAX_BUFFER_SIZE);
        memset(filename, '\0', MAX_BUFFER_SIZE);
        fread(filename, 1, DIR_FILENAME_SIZE, file);

        //Read in the filename
        fseek(file, cur + DIR_EXTENSION_POS, SEEK_SET);
        char* extension = malloc(MAX_BUFFER_SIZE);
        memset(extension, '\0', MAX_BUFFER_SIZE);
        fread(extension, 1, DIR_EXTENSION_SIZE, file);

        //Read in creation date (little endian - turn it into a big endian int)
        fseek(file, cur + DIR_CREATION_DATE_POS, SEEK_SET);
        byte1 = 0;
        byte2 = 0;
        fread(&byte1, 1, 1, file);
        fread(&byte2, 1, 1, file);
        int createDate = (byte1 << 0) + (byte2 << 8);

        //Read in creation time (little endian - turn it into a big endian int)
        fseek(file, cur + DIR_CREATION_TIME_POS, SEEK_SET);
        byte1 = 0;
        byte2 = 0;
        fread(&byte1, 1, 1, file);
        fread(&byte2, 1, 1, file);
        int createTime = (byte1 << 0) + (byte2 << 8);

        //Print what we know!
        char* trimFName = trimWhitespace(filename);
        printf("%c %10d %20s.%3s %10d %10d\n", type, size, trimFName, extension, createDate, createTime);

        //Clean up memory
        free(filename);

      }
    }
    cur = cur + DIR_OFFSET_SIZE;
    fseek(file, cur, SEEK_SET);
    fread(&dirStatus, 1, 1, file);
  }
}


//EOF

