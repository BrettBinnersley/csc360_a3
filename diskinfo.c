/*
Brett Binnersley, V00776751
Csc 360, Assignment #3
Part 1

March, 2015
*/

#include "shared.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("To run: diskinfo <FILE>\n");
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







