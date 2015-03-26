/*
Brett Binnersley, V0076751
Csc 360, Assignment #3
Part 1

March, 2015
*/

#include <stdio.h>
#include <stdlib.h>
#include "locations.h"

//large number used for allocating char arrays (nothing we get will be larger)
#define MAX_BUFFER_SIZE 4096


void getOSName(FILE* fp, char* osname)
{
  fseek(fp, 3L, SEEK_SET);
  fread(osname, 1, 8, fp);
}

void getDiskLabel(FILE* fp, char* label)
{
  (*label) = "HELLO WORLD";
}


int getTotalSize(FILE* fp)
{
  int *tmp1 = malloc(sizeof(int));
  int *tmp2 = malloc(sizeof(int));
  int retVal;
  fseek(fp, 19L, SEEK_SET);
  fread(tmp1, 1, 1, fp);
  fread(tmp2, 1, 1, fp);
  retVal = *tmp1 + ( (*tmp2) <<8 );
  free(tmp1);
  free(tmp2);
  return retVal;
}

int getFreeSpace(FILE* fp)
{
  return 0;
  // int n = 2;
  // int base = 512;
  // int tmp1 = 0;
  // int tmp2 = 0;
  // for (n = 2; n <= (numSectors-1-33+2); n++)
  // {
  //   if (n % 2 == 0)
  //   {
  //     fseek(fp, base + 3*n/2, SEEK_SET);
  //     fread(&tmp1, 1, 1, fp);
  //     fread(&tmp2, 1 ,1, fp);
  //     tmp2 = tmp2 & 0x0F;
  //     result = tmp2 << 8 + tmp1;
  //   }
  //   else
  //   {
  //     //FILL OUT CODE HERE>>>>??????????
  //   }

  //   if (result == 0x00)
  //   {
  //     counter ++;
  //   }
  // }
  // //Add some sort of return value here
}

int getNumFilesInRoot(FILE* fp)
{
  return 0;
  // base = 9728;
  // int cur = base;
  // int offset = 32;

  // fseek(fp, base, SEEK_SET);
  // char tmp;
  // char tmp2;
  // fread(&tmp,1,1,fp);
  // while(tmp != 0x00)
  // {
  //   if (tmp != 0xE5)
  //   {
  //     //Some code here
  //   }
  //   cur = cur + offset;
  //   fseek(fp, cur, SEEK_SET);
  //   fread(&tmp,1,1,fp);
  // }
}

// # of fat copies
int getNumFatCopies(file)
{
  return 0;
}

// # of sectors per fat
int getNumSectorsPerFat(file)
{
  return 0;
}



int main(int argc, char *argv[])
{
  if(argc < 1)
  {
    printf("Incorrect argument counter\n");
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

  char osName[MAX_BUFFER_SIZE];
  memset(&osName, '/0', MAX_BUFFER_SIZE);
  getOSName(file, &osName);

  char diskLabel[MAX_BUFFER_SIZE];
  memset(&osName, '/0', MAX_BUFFER_SIZE);
  getDiskLabel(file, &diskLabel);

  int totalSize = getTotalSize(file);
  int freeSize = getFreeSpace(file);
  int numFiles = getNumFilesInRoot(file);
  int numFat = getNumFatCopies(file);
  int sectorsPerFat = getNumSectorsPerFat(file);

  printf("OS Name: %s\n", osName);
  printf("Label of the disk: %s\n", diskLabel);
  printf("Total size of the disk: %d\n", totalSize);
  printf("Free size of the disk: %d\n", freeSize);
  printf("\n==============\n");
  printf("The number of files in the root directory (not including subdirectories): %d\n", numFiles);
  printf("\n=============%s\n");
  printf("Number of FAT copies %s\n", numFat);
  printf("Sectors per Fat: %d\n", sectorsPerFat);

  fclose(file);
  return 0;
}


//EOF







