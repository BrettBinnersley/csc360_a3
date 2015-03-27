/*
Brett Binnersley, V00776751
Csc 360, Assignment #3

Functions that are shared between multiple parts (ie: pt1, pt 2, pt 3 etc.)
[Reuse functionality - we don't have to reprogram them, or copy + paste them each time]


March, 2015
*/

#ifndef SHARED_FUNCTIONS_INCLUDED
#define SHARED_FUNCTIONS_INCLUDED

  //Includes used in all of our files
  #include <string.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include "constants.h"

  //Shared function def's
  void getOSName(FILE* file, char* osname);
  void getDiskLabel(FILE* file, char* label);
  int getSectorCount(FILE* file);
  int getFreeSectorCnt(FILE* file, int numSectors);
  int getNumFilesInRoot(FILE* file);
  int getNumFatCopies(FILE* file);
  int getNumSectorsPerFat(FILE* file);
  int getSizeFile(FILE* file, int location); //Read in little endian, return int in big endian
  int getSectorPos(int firstLogClus);
  int getNextLogClusFromFAT(FILE* file, int logicalCluster);
  int min(int num1, int num2);
  unsigned int convertShort(unsigned int bigEnd); //Functions for converting big to little endian (for I/O)
  unsigned int convertInt(unsigned int bigEnd);
  int getFileSize(FILE* file);
  int GetFreeFAT(FILE* file, int notFAT); //Get a free FAT. undefined behavior if none exist (check beforehand)

  //String functions
  char* trimWhitespace(char* string);
  char* covertToUpper(char* string); //Note: the returned string needs to be freed
  char* extractLabelName(FILE* file); //Extract volume label (name) from the image


//End include guard
#endif

//EOF
