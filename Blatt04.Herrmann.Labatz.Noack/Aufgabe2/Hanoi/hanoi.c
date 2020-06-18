#include <stdlib.h>
#include <stdio.h>

void hanoimoves(short groese, short start, short end)
{
  if (groese > 1)
  {
    hanoimoves(groese - 1, start, 6 - start - end);
    printf("(%i,%i) ", start, end);
    hanoimoves(groese - 1, 6 - start - end, end);
  } else
  {
    printf("(%i,%i) ", start, end);
  }
}

int main(int argc, char const *argv[])
{
  short n;
  if (argc != 2 || sscanf(argv[1], "%hi", &n) != 1 || n <= 0)
  {
    fprintf(stderr, "%s: Wähle ein valides Argument\n", argv[0]);
    return EXIT_FAILURE;
  }

  hanoimoves(n, 1, 2);

  printf("\n");

  return EXIT_SUCCESS;
}
