#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
  int n;
  char dict[] = {'a', 'b', 't', 'n', 'v', 'f', 'r'};
  for (char r = 0; r < 32; r++)
  {
    for (char i = 0; i < 4; i++)
    {
      n = r + i * 32;
      if (n >= 33 && n <= 126)
      {
        printf("%3d%5c", n, n);
        // führende "0" werden mit Leerzeichen gefüllt
      } else if (n > 6 && n < 14)
      {
        printf("%3d   \\%c", n, dict[n - 7]);
      } else
      {
        printf("%3d%s\\%d", n, n < 10 ? "   " : (n < 100 ? "  " : " "), n);
      }
      if (i < 3)
      {
        printf("\t");
      }
    }
    printf("\n");
  }
  return EXIT_SUCCESS;
}
