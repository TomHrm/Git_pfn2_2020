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
  bitvector = malloc(BITVECTOR_MAX_WIDTH +1);
  if (bitvector == NULL)
  {
    fprintf(stderr, "malloc failed: %d", BITVECTOR_MAX_WIDTH);
    exit(EXIT_FAILURE);
  }
  for (int i = BITVECTOR_MAX_WIDTH -1; i >= 0; i--)
  {
    if ((BITVECTOR_MAX_WIDTH - i) % 9 == 0)
    {
      bitvector[i] = (char) 32;
    }
    else
    {
      bitvector[i] = ((char) n % 2) + '0';
      n /= 2;
      
    }
  }
  bitvector[BITVECTOR_MAX_WIDTH] = '\0';
  return bitvector;
}

int bitvector_validate(const char *bitvector)
{
  if ( !(NUMOFBITS <= strlen(bitvector) && strlen(bitvector) <= BITVECTOR_MAX_WIDTH))
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
  unsigned int summe = 0;
  unsigned int potenz = 1;
  for (int i = strlen(bitvector) ; i >= 0 ; i--)
  {
    if (bitvector[i-1] == '1')
    {
      summe += potenz;
      potenz *= 2;
    }
    else if (bitvector[i-1] == '0')
    {
      potenz *= 2;
    }
  }
  return summe;
}

int main(int argc, char *argv[])
{
  //printf("%d,%s\n",argc, argv[0]);
  //printf("%s\n", decimal2bitvector(256));
  //printf("%d\n",bitvector_validate(decimal2bitvector(256)));
  //printf("%d",bitvector2decimal(decimal2bitvector(256)));
  
unsigned int i =1;

while (i< 16777000)
{
printf("%s, %d, %d\n", decimal2bitvector(i), bitvector_validate(decimal2bitvector(i)), bitvector2decimal(decimal2bitvector(i)));
i = i*2;
}


 
  return EXIT_SUCCESS;
}

