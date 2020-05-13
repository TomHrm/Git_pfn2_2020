#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "bintodec.h"

/*
   Diese Funktion vergleicht zwei validierte Bitvektoren und liefert genau
   dann true zur"uck, wenn die Bitvektoren die gleichen Zahlen
   repr"asentieren.
   FRAGE 1: Warum kann man hierf"ur nicht die C-Funktion strcmp verwenden?
*/

static bool bitvector_equal(const char *bv0,const char *bv1)
{
  unsigned long idx0, idx1;

  for (idx0 = idx1 = 0; bv0[idx0] != '\0' && bv1[idx1] != '\0'; /* Nothing */)
  {
    if (bv0[idx0] == ' ' || bv1[idx1] == ' ')
    {
      if (bv0[idx0] == ' ')
      {
        idx0++;
      }
      if (bv1[idx1] == ' ')
      {
        idx1++;
      }
    } else
    {
      if (bv0[idx0] != bv1[idx1])
      {
        fprintf(stderr,"bv0[%lu] = %c != %c = bv1[%lu]\n",
                        idx0,bv0[idx0],bv1[idx1],idx1);
        return false;
      }
      idx0++;
      idx1++;
    }
  }
  return true;
}

/* Diese Funktion verifiziert, dass die Konversion einer Dezimalzahl n
   in den Bitvektor und die Konversion des Bitvektors in eine
   Dezimalzahl die urspr"ungliche Dezimalzahl n liefert.
   FRAGE 2: Warum ist es notwendig, an drei Stellen die Funktion free
   aufzurufen? */

static int verify_conversion(unsigned int decimal)
{
  char *bitvector = decimal2bitvector(decimal);
  unsigned int converted_decimal;

  if (bitvector_validate(bitvector) != 0)
  {
    free(bitvector);
    return -1;
  }
  converted_decimal = bitvector2decimal(bitvector);
  if (decimal != converted_decimal)
  {
    fprintf(stderr,"bitvector = %s, decimal = %u != %u = value\n",
                    bitvector,decimal,converted_decimal);
    free(bitvector);
    return -1;
  }
  free(bitvector);
  return 0;
}

int main(int argc,char *argv[0])
{
  const int precision = 1 + (int) log10((double) UINT_MAX);

  if (argc == 1)
  {
    unsigned long trial;
    unsigned int decimal, display;

    /* FRAGE 3: F"ur welche Werte werden in der folgenden for-Schleife
       Tests durchf"uhrt? */
    for (decimal = 0, display = 1; decimal < 16777216; decimal++)
    {
      if (verify_conversion(decimal) != 0)
      {
        return EXIT_FAILURE;
      }
      /* FRAGE 4: Wozu dient diese if-Anweisung? */
      if (display == decimal)
      {
        char *this_bitvector = decimal2bitvector(decimal);
        printf("%*u = %s\n",precision,decimal,this_bitvector);
        free(this_bitvector);
        display *= 2;
      }
    }
    /* FRAGE 5: Aus welchem Wertebereich stammen die Zahlen, f"ur die
       in der folgenden Schleife Tests durchgef"uhrt werden? */
    srand48(366292341);
    for (trial = 0; trial < 100; trial++)
    {
      const unsigned int decimal = drand48() * UINT_MAX;
      if (verify_conversion(decimal) != 0)
      {
        return EXIT_FAILURE;
      }
    }
  } else
  {
    int idx;
    /* FRAGE 6: F"ur welche Strings werden in der folgenden Zeile
       Tests durchgef"uhrt? */
    for (idx = 1; idx < argc; idx++)
    {
      unsigned int decimal;
      char *this_bitvector;

      if (bitvector_validate(argv[idx]) != 0)
      {
        return EXIT_FAILURE;
      }
      decimal = bitvector2decimal(argv[idx]);
      this_bitvector = decimal2bitvector(decimal);
      if (!bitvector_equal(argv[idx],this_bitvector))
      {
        fprintf(stderr,"bitvectors %s and %s do not represent the same decimal "
                       "value %u\n",argv[idx],this_bitvector,decimal);
        free(this_bitvector);
        return EXIT_FAILURE;
      }
      printf("%*u = %s\n",precision,decimal,this_bitvector);
      free(this_bitvector);
    }
  }
  return EXIT_SUCCESS;
}
