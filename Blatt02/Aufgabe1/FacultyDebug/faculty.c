#include <stdio.h>


int main(int argc, char *argv[])
{
  long /* cnt */ value = 1;
  int n;

  if (argc != 2 || sscanf(argv[1],"%d",&n) != 1 || n < 0)
  {
    fprintf(stderr," Usage: {} <positive_integer %s>\t\n" , argv[1]);
    return EXIT_FAILURE;
  }
  for (int count = 1; count <= n; count++)
    {
      value = value * count;
    }
  printf("%d\t%ld\n",n,value);
  return EXIT_SUCCESS;
}