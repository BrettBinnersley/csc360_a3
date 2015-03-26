/*
Brett Binnersley, V00776751
Csc 360, Assignment #3
Part 1

March, 2015
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "locations.h"

//large number used for allocating char arrays (nothing we get will be larger)
#define MAX_BUFFER_SIZE 4096


void getOSName(FILE* file, char* osname)
{
  fseek(file, 3L, SEEK_SET);
  fread(osname, 1, 8, file);
}

void getDiskLabel(FILE* file, char* label)
{
  fseek(file, LABEL_POS, SEEK_SET);
  fread(label, 1, LABEL_SIZE, file);
}


int getSectorCount(FILE* file)
{
  int *tmp1 = malloc(sizeof(int));
  int *tmp2 = malloc(sizeof(int));
  int retVal;
  fseek(file, 19L, SEEK_SET);
  fread(tmp1, 1, 1, file);
  fread(tmp2, 1, 1, file);
  retVal = *tmp1 + ( (*tmp2) << 8 );
  free(tmp1);
  free(tmp2);
  return retVal;
}

//Fat are duplicated
//Each entry has 12 bits in FAT table
int getFreeSectorCnt(FILE* file, int numSectors)
{
  int counter = 0; //The number of free sectors (of SECTOR_SIZE)
  int n = FAT_RESERVED_CNT;
  int base = FAT_FIRST_POS;
  int tmp1 = 0; //byte 1
  int tmp2 = 0; //byte 2
  int tmpByte = 0;
  int result = FAT_UNUSED;
  for (n = 2; n <= (numSectors-1-33+2); n++)
  {
    if (n % 2 == 0)
    {
      fseek(file, base + 3*n/2, SEEK_SET);
      fread(&tmp1, 1, 1, file);
      fread(&tmp2, 1 ,1, file);
      tmpByte = tmp2 & 0x0F; //Get the low 4 bits
      result = (tmpByte << 8) + tmp1;
    }
    else
    {
      //Seek (from current position) the next byte.
      fread(&tmp1, 1, 1, file);
      tmpByte = tmp2 & 0xF0; //high 4 bits
      result = (tmpByte >> 4) + (tmp1 << 4);
    }

    if (result == FAT_UNUSED)
    {
      counter ++;
    }
  }
  return counter;
}

int getNumFilesInRoot(FILE* file)
{
  return 0;
  int base = (SECTOR_ROOT_FIRST * SECTOR_SIZE); //First byte in the root directory
  int cur = base; //Current pos in the directory
  int offset = DIR_OFFSET_SIZE;
  int numFiles = 0;

  fseek(file, base, SEEK_SET);
  char dirStatus = 0;
  fread(&dirStatus, 1, 1, file); //Read in 1 char from the file at a time
  while(dirStatus != DIR_FREE) //This dir is free as well as all other dirs in the CWD
  {
    if (dirStatus != DIR_EMPTY) //This dir has something inside of it
    {
      /* Locate the byte for the current entry's attribute */
      //fseek(fp, ?, SEEK_SET);
      char tmp2;
      //fread(&tmp2,1,1,fp);

      /* What is the attribute of the entry ? */
      /* if not 0x0F(not part of a long file name), not suddirectory, not volume label, then it is a file. */
      //if( ??? )
      //{
      //  counter ++;
      //}
    }
    cur = cur + offset;
    fseek(file, cur, SEEK_SET);
    fread(&dirStatus, 1, 1, file);
  }
  return numFiles;
}

// # of fat copies
int getNumFatCopies(FILE* file)
{
  fseek(file, NUMBER_FAT_POS, SEEK_SET);
  int numCopies = 0;
  fread(&numCopies, 1, NUMBER_FAT_SIZE, file);
  return numCopies;
}

// # of sectors per fat
int getNumSectorsPerFat(FILE* file)
{
  fseek(file, SECTOR_PER_FAT_POS, SEEK_SET);
  int numSects = 0;
  fread(&numSects, 1, SECTOR_PER_FAT_SIZE, file);
  return numSects;
}



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

  char* osName = malloc(MAX_BUFFER_SIZE);
  memset(osName, '\0', MAX_BUFFER_SIZE);
  getOSName(file, osName);

  char* diskLabel = malloc(MAX_BUFFER_SIZE);
  memset(diskLabel, '\0', MAX_BUFFER_SIZE);
  getDiskLabel(file, diskLabel);

  int numFat = getNumFatCopies(file);
  int sectorsPerFat = getNumSectorsPerFat(file);

  int sectorCnt = getSectorCount(file);
  int totalSize = sectorCnt * SECTOR_SIZE;
  int freeSize = getFreeSectorCnt(file, sectorCnt) * SECTOR_SIZE;
  int numFiles = getNumFilesInRoot(file);


  printf("OS Name: %s\n", osName);
  printf("Label of the disk: %s\n", diskLabel);
  printf("Total size of the disk: %d bytes\n", totalSize);
  printf("Free size of the disk: %d bytes\n", freeSize);
  printf("==============\n");
  printf("The number of files in the root directory (not including subdirectories): %d\n", numFiles);
  printf("=============\n");
  printf("Number of FAT copies: %d\n", numFat);
  printf("Sectors per Fat: %d\n", sectorsPerFat);

  //Clean up memory
  fclose(file);
  free(osName);
  free(diskLabel);
  return 0;
}


//EOF







