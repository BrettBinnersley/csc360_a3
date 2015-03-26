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
  char* trimWhitespace(char* string); //Note: still must free original string (else memory leak!)


//End include guard
#endif

//EOF
