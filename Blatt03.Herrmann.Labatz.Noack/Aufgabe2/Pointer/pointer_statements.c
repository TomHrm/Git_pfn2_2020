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

  strings = malloc(sizeof(*strings) * 3);
  strings[0] = &c;
  strings[1] = string2;
  strings[2] = "third string";

  a = 7;
  nump = &a;
  *nump = 5;
  b = &a;
  num = (int) (*(strings + 1))[1];
}
/*
Typen der deklarierten Variablen
  c: char-Array
  string: char-Array pointer
  string2: Char-Array
  strings: Double pointer
  num: int
  nump: int pointer
  a: int
  b: int pointer

Belegung nach Ausführung des Programms
  c = 'W'
  string2 = {'A','B', NULL, '\0'}
  string2[0] = A
  *strings = Liste von Adressen
  strings[2][2] = i
  string2[0] = 'A'
  num = 66

*strings[0] == *(string + 6) = True
b == *nump = True
b = nump = True
string == strings[1] = False

*/