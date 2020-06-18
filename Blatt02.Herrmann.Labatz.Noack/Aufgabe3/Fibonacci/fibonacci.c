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
  FibonacciNumberType temp;
  FibonacciNumberType a = 0;  // vorherige Element
  FibonacciNumberType b = 1;  // jetzige Element

  FibonacciNumberType_print(0, a);
  FibonacciNumberType_print(1, b);

  int i = 2;
  while (b < FibonacciNumberType_max / 2)
  {
    temp = b + a;
    a = b;
    b = temp;
    FibonacciNumberType_print(i, b);
    i++;
  }

  return EXIT_SUCCESS;
}
