/*
Brett Binnersley, V00776751
Csc 360, Assignment #3
Part 4

March, 2015
*/

#include "shared.h"
#include <sys/time.h>

//Forward Decl so that we can program main first
void InsertFile(FILE* file, FILE* insert, int insertSize, char* insertFilename);
void AddToRootDirectory(FILE* file, char* buffer, int readSize);
void writeDataFromFAT(char buffer[], int size, FILE* file, int FAT);
void writeNextFat(FILE* file, int FAT, int nextFAT);

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    printf("diskinfo <FILESYSTEM> <FILENAME>\n");
    return 0;
  }
  char* fileName = argv[1];

  //Open both the input file and the filesystem file
  FILE* file = 0;
  file = fopen(fileName, "r+"); //Note r+ means we can read AND write to the file at the same time.
  if(file == 0)
  {
    printf("File not found. (disk image)\n");
    return 0;
  }

  char* insertFilename = argv[2];
  FILE* insertFile = 0;
  insertFile = fopen(insertFilename, "r");
  if(insertFile == 0)
  {
    printf("File not found. (insert file)\n");
    fclose(file);
    return 0;
  }


  //Ensure there is enough space
  int sectorCnt = getSectorCount(file);
  int freeSize = getFreeSectorCnt(file, sectorCnt) * SECTOR_SIZE;
  int insertFileSize = getFileSize(insertFile);
  if(insertFileSize > freeSize)
  {
    printf("Not enough free space in the disk image.\n");
    fclose(insertFile);
    fclose(file);
    return 0;
  }

  //Insert the file into the filesystem.
  InsertFile(file, insertFile, insertFileSize, insertFilename);
  fclose(insertFile);
  fclose(file);
  return 0;
}


void InsertFile(FILE* file, FILE* insert, int insertSize, char* insertFilename)
{
  //////////////////////////////
  //Add it to the root directory
  //////////////////////////////
  int base = (SECTOR_ROOT_FIRST * SECTOR_SIZE);
  int cur = base;
  int numFiles = 0;
  fseek(file, base, SEEK_SET);
  int dirStatus = 0;
  fread(&dirStatus, 1, 1, file);
  while(dirStatus != DIR_FREE)
  {
    cur = cur + DIR_OFFSET_SIZE;
    fseek(file, cur, SEEK_SET);
    fread(&dirStatus, 1, 1, file);
  }

  char directory[MAX_BUFFER_SIZE];
  memset(directory, '\0', MAX_BUFFER_SIZE);

  //Find out the filename
  char* filename = 0;
  char* extension = 0;
  char* pch = strtok (insertFilename,".");
  filename = pch;
  if (pch != NULL)
  {
    pch = strtok (NULL, ".");
    extension = pch;
  }
  else
  {
    extension = "   ";
  }
  filename = covertToUpper(filename);
  extension = covertToUpper(extension);

  //Find out the times
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned int createTime = tv.tv_usec / 100000;
  unsigned int createDate = 0;
  unsigned int FAT = GetFreeFAT(file, -1);
  unsigned int filesize = insertSize;

  const char NULL_CHAR = '\0';

  //write fname + ext
  fseek(file, cur, SEEK_SET);
  sprintf(directory, "%-8s%-3s%c%c%c",
    filename,
    extension,
    NULL_CHAR,
    NULL_CHAR,
    NULL_CHAR);
  fwrite(directory, sizeof(char), 14, file);

  //Write times
  unsigned int ct = convertShort(createTime);
  char cta = (char) (ct >> 8);
  char ctb = (char) ct;
  unsigned int cd = convertShort(createDate);
  char cda = (char) (cd >> 8);
  char cdb = (char) cd;
  sprintf(directory, "%c%c%c%c%c%c%c%c%c%c%c%c",
    cta, ctb,
    cda, cdb,
    NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
    cta, ctb,
    cda, cdb);
  fwrite(directory, sizeof(char), 12, file);

  //Write FAT + sizes
  unsigned int cfat = convertShort(FAT);
  char cfata = (char) (cfat >> 8);
  char cfatb = (char) cfat;

  unsigned int csize = convertInt(filesize);
  char fsa = (char) (csize >> 24);
  char fsb = (char) (csize >> 16);
  char fsc = (char) (csize >> 8);
  char fsd = (char) csize;

  sprintf(directory, "%c%c%c%c%c%c",
    cfata, cfatb,
    fsa, fsb, fsc, fsd);
  fwrite(directory, sizeof(char), 6, file);

  /////////////////////////////////
  //Add references to the FAT table
  /////////////////////////////////
  int size = 0;
  int nextFAT = FAT;
  while(1)
  {
    FAT = nextFAT;
    int readSize = min(insertSize - size, SECTOR_SIZE); //[0, 512]

    char* buffer = malloc(MAX_BUFFER_SIZE);
    memset(buffer, '\0', MAX_BUFFER_SIZE);
    fread(buffer, 1, readSize, insert); //Read in X bytes from input file into the buffer
    writeDataFromFAT(buffer, readSize, file, FAT); //Write to data section.
    free(buffer);

    size += readSize;
    if(size >= insertSize)
    {
      nextFAT = FAT_LAST_CLUSTER_END;
    }
    else
    {
      nextFAT = GetFreeFAT(file, FAT); //Get the next fat, not 0
    }

    //Write to the FAT
    writeNextFat(file, FAT, nextFAT);


    //Finished with file
    if(size >= insertSize)
    {
      break;
    }

  }
  printf("Successfully Inserted File!\n");
}


void writeDataFromFAT(char buffer[], int size, FILE* file, int FAT)
{
  int sectorPos = getSectorPos(FAT);
  fseek(file, sectorPos, SEEK_SET);
  fwrite(buffer, sizeof(char), size, file);
}

void writeNextFat(FILE* file, int FAT, int nextFAT)
{
  nextFAT = convertShort(nextFAT);
  char nextFAT1 = (char) (nextFAT >> 8);
  char nextFAT2 = (char) (nextFAT);
  char c1 = '\0';
  char c2 = '\0';

  if (FAT % 2 == 0)
  {
    fseek(file, FAT_FIRST_POS + 3*FAT/2, SEEK_SET);
    fread(&c1, 1, 1, file);
    fread(&c2, 1, 1, file);
    tmp2 = tmp2 & 0x0F; //Get the low 4 bits
    result = (tmp2 << 8) + tmp1;
  }
  else
  {
    fseek(file, FAT_FIRST_POS + 3*FAT/2, SEEK_SET);
    fread(&tmp1, 1, 1, file);
    fread(&tmp2, 1 ,1, file);
    tmp1 = tmp1 & 0xF0; //high 4 bits
    result = (tmp1 >> 4) + (tmp2 << 4);
  }

  nextFAT = nextFAT | val;
  fseek(file, FAT_FIRST_POS + 3*FAT/2, SEEK_SET);
  fwrite(&val, sizeof(char), 2, file); //guaranteed to be 2 bytes

}


    if (n % 2 == 0)
    {
      fseek(file, base + 3*n/2, SEEK_SET);
      fread(&tmp1, 1, 1, file);
      fread(&tmp2, 1 ,1, file);
      tmp2 = tmp2 & 0x0F; //Get the low 4 bits
      result = (tmp2 << 8) + tmp1;
    }
    else
    {
      fseek(file, base + 3*n/2, SEEK_SET);
      fread(&tmp1, 1, 1, file);
      fread(&tmp2, 1 ,1, file);
      tmp1 = tmp1 & 0xF0; //high 4 bits
      result = (tmp1 >> 4) + (tmp2 << 4);
    }



//EOF



