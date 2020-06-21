#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef SMALLVALUES
typedef unsigned short FibonacciNumberType;
#define FibonacciNumberType_max USHRT_MAX
#define FibonacciNumberType_print(I, F) printf("%d\t%hu\n", I, F)
#else
#ifdef MEDIUMVALUES
typedef unsigned int FibonacciNumberType;
#define FibonacciNumberType_max UINT_MAX
#define FibonacciNumberType_print(I, F) printf("%d\t%u\n", I, F)
#else
typedef unsigned long FibonacciNumberType;
#define FibonacciNumberType_max ULONG_MAX
#define FibonacciNumberType_print(I, F) printf("%d\t%lu\n", I, F)
#endif
#endif

int main(void)
{
  int i; /* Zaehler f"ur die Nummer der Fibonacci-Zahl */
  /* Hier folgt nun der Programmcode zur Berechnung der Fibonacci-Zahlen.
     Die Variable i darf verwendet werden.
     Es d"urfen nur Variablen vom Typ FibonacciNumberType deklariert werden.
  */
  return EXIT_SUCCESS;
}
