#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  char c, *string, string2[3] = {0}, **strings;
  int num, *nump, a, *b;

  string = "Hallo Welt";
  c = string[6];

  string2[0] = 'A';
  string2[1] = 'B';

  strings = malloc(sizeof (*strings) * 3);
  strings[0] = &c;
  strings[1] = string2;
  strings[2] = "third string";

  a = 7;
  nump = &a;
  *nump = 5;
  b = &a;
  num = (int) (*(strings + 1))[1];
}
