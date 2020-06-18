#include <stdio.h>
#include <limits.h>
#define BITS(X) (sizeof(X) * 8)

int main(void)
{
  printf("# type\tbits\tmin\tmax\tmax\n");
  printf("char\t%u\t%d\t%d\t%3.2e\n", (int) BITS(char), SCHAR_MIN, SCHAR_MAX,
         (float) SCHAR_MAX);
  printf("short\t%u\t%d\t%d\t%3.2e\n", (int) BITS(short), SHRT_MIN, SHRT_MAX,
         (float) SHRT_MAX);
  printf("int\t%u\t%d\t%d\t%3.2e\n", (int) BITS(int), INT_MIN, INT_MAX,
         (float) INT_MAX);
  printf("long\t%u\t%ld\t%ld\t%3.2e\n", (int) BITS(long), LONG_MIN, LONG_MAX,
         (float) LONG_MAX);
  printf("unsigned char\t%u\t%u\t%d\t%3.2e\n", (int) BITS(char), 0, UCHAR_MAX,
         (float) UCHAR_MAX);
  printf("unsigned short\t%u\t%d\t%d\t%3.2e\n", (int) BITS(short), 0, USHRT_MAX,
         (float) USHRT_MAX);
  printf("unsigned int\t%u\t%d\t%u\t%3.2e\n", (int) BITS(int), 0, UINT_MAX,
         (float) UINT_MAX);
  printf("unsigned long\t%u\t%u\t%lu\t%3.2e\n", (int) BITS(long), 0, ULONG_MAX,
         (float) ULONG_MAX);
  return 0;
}
