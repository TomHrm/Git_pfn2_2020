#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
//#include "bintodec.h"

#define NUMOFBITS 32 /* number of bits in bitvector */
#define BITVECTOR_MAX_WIDTH (NUMOFBITS + NUMOFBITS/CHAR_BIT - 1)


char *decimal2bitvector(unsigned int n)
{
    char *bitvector;
    
    bitvector = (char*) malloc(BITVECTOR_MAX_WIDTH+1);
     if (bitvector == NULL)
  {
    fprintf(stderr, "malloc failed: %d", BITVECTOR_MAX_WIDTH);
    exit(EXIT_FAILURE);
  }
    for (int i = 0, potenz = NUMOFBITS - 1; i <= BITVECTOR_MAX_WIDTH; i++)
    {
        if (i % 9 == 8)
        {
            bitvector[i] = ' ';
        }
        else
        {
            if (n >= ((unsigned int) 1)  << potenz)
            {
                n -= ((unsigned int) 1) << potenz;
                bitvector[i] = '1';
            } 
            else
            {
                bitvector[i] = '0';
            }
            potenz--;
        }
    }
    
    bitvector[BITVECTOR_MAX_WIDTH] = 0x00;

    return bitvector;
}

int bitvector_validate(const char *bitvector)
{
  if ( !(NUMOFBITS <= strlen(bitvector) && 
    strlen(bitvector) <= BITVECTOR_MAX_WIDTH))
  {
    fprintf(stderr,"Ungueltige Eingabe Laenge.\n");
    return -1;
  }
  int valid = 0;

  for (int i = 0; i< strlen(bitvector); i++)
  {
    if (bitvector[i] == '0' || bitvector[i] == '1')
    {
      valid++;
    }
    else if (bitvector[i] != ' ')
    {
      fprintf(stderr,"Ungueltiger Charakter.\n");
      return -1;
    }
  }
  
  if (valid != NUMOFBITS)
  {
    fprintf(stderr,"Ungueltige Bitanzahl.\n");
    return -1;
  }
  
  return 0;
}

unsigned int bitvector2decimal(const char *bitvector)
{
    int i = strlen(bitvector);
    int potenz = 0;
    unsigned int summe = 0;
    while (i >= 0)
    {
        if (bitvector[i] == '1')
        {
            summe += ((unsigned int) 1) << potenz;
            potenz++;
        }
        else if (bitvector[i] == '0'  )
        {
            potenz++;
        }
        i--;
    }
    return summe;
}
