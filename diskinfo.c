// loop through the FAT table
// Each entry has 12 bits in FAT table
int getFreeSpace(FILE* fp)
{
  int n = 2;  // logical number of the first sector in Data Area
  int base = 512; // the first byte of the FAT table

  int tmp1 = 0;
  int tmp2 = 0;

  // The logical number for all the sectors in Data Area is from 2 to 2848
       for (n = 2; n <= (numSectors-1-33+2); n++)
  {
    // given logical no. of sector in data area
    // where is the corresponding entry in FAT table ?
    // For the algorithm, refer to the last page of FAT Description:
    // http://webhome.csc.uvic.ca/~wkui/Courses/CSC360/FAT12Description.pdf

    // if the logical sector number is even
    if (n % 2 == 0)
    {
      fseek(fp, base + 3*n/2, SEEK_SET);
      fread(&tmp1, 1, 1, fp);  // get all 8 bits
      fread(&tmp2,1 ,1, fp);
      tmp2 = tmp2 & 0x0F;   // get the low 4 bits

      // Then apply "Little Endian": (4 bits)**** + (8 bits)********
      result = tmp2 << 8 + tmp1;
    }

    // if the logical sector number is odd
    else
    {
      ;

    }

    if (result == 0x00)  // if the value is 0x00, that sector is free/unused
    {
      counter ++;
    }

  }

}

// loop through the root directory
// Each entry has 32 bytes in root directory
int countRootDirFiles(FILE* fp)
{
  base = 9728;  // the first byte of the root directory

  int cur = base;   // point to the first byte of the current entry
  int offset = 32;  // Each entry has 32 bytes in root directory

  fseek(fp, base, SEEK_SET);
  char tmp;
  char tmp2;
  fread(&tmp,1,1,fp);

  /* Read "notes on directory entries" for the correct conditions of how we identify a file */
  /* Why 0x00 here? */
  while(tmp != 0x00)
  {
    // Search for files
    // Why 0xE5 ?
    if (tmp != 0xE5)
    {
      /* Locate the byte for the current entry's attribute */
      //fseek(fp, ?, SEEK_SET);
      //fread(&tmp2,1,1,fp);

      /* What is the attribute of the entry ? */
      /* if not 0x0F(not part of a long file name), not suddirectory, not volume label, then it is a file. */
      //if( ??? )
      //{
      //  counter ++;
      //}

    }

    // Go to next entry in Root Directory
    cur = cur + offset;
    fseek(fp, cur, SEEK_SET);
    fread(&tmp,1,1,fp);
  }

}















