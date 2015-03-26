/*
Brett Binnersley, V00776751
Csc 360, Assignment #3

Functions that are shared between multiple parts (ie: pt1, pt 2, pt 3 etc.)
[Reuse functionality - we don't have to reprogram them, or copy + paste them each time]

March, 2015
*/

#include "shared.h"
#include <ctype.h>

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
  int base = (SECTOR_ROOT_FIRST * SECTOR_SIZE); //First byte in the root directory
  int cur = base; //Current pos in the directory
  int numFiles = 0;

  fseek(file, base, SEEK_SET);
  int dirStatus = 0;
  fread(&dirStatus, 1, 1, file); //Read in 1 char from the file at a time
  while(dirStatus != DIR_FREE) //This dir is free as well as all other dirs in the CWD
  {
    if (dirStatus != DIR_EMPTY) //This dir has something inside of it
    {
      /* Locate the byte for the current entry's attribute */
      int attributes;
      fseek(file, cur + DIR_ATTRIBUTES_POS, SEEK_SET);
      fread(&attributes, 1, DIR_ATTRIBUTES_SIZE, file);
      /* What is the attribute of the entry ? */
      /* if not 0x0F(not part of a long file name), not suddirectory, not volume label, then it is a file. */
      if(attributes != DIR_LONG_FILENAME &&
         !(attributes & DIR_SUBDIRECTORY) &&
         !(attributes & DIR_VOLUME_LABEL))
      {
        ++numFiles;
      }
    }
    cur = cur + DIR_OFFSET_SIZE;
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

char* trimWhitespace(char* string)
{
  char* endChar;

  // Trim leading space
  while(isspace(*string))
  {
    ++string;
  }

  //If everything was a space.
  if(*string == 0)
  {
    return string;
  }

  // Trim trailing space
  endChar = string + strlen(string) - 1;
  while(endChar > string && isspace(*endChar))
  {
    --endChar;
  }

  // Write new null terminator
  *(endChar + 1) = 0;

  return string;
}


//EOF
