#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
  unsigned long n; //die Obergrenze der Menge M
  unsigned long steps = 0; //die Anzahl der Schritte
  if(argc != 2 || (sscanf(argv[1], "%lu", &n)) != 1)
  {
  fprintf(stderr, "Ungültiges Argument");
  exit(EXIT_FAILURE);
  }
  char *arr = calloc(n, sizeof *arr);
  //Char-Array mit Kapazität, in dem '1' Index 
  //eines in M enthaltenen Elementes steht, sonst '0'

  int i = 1; //Laufvariable
  arr[i] = 1;

  steps += 4; //zuweisungen
  steps += 3; //Funktionsaufrufe
  
  //bis hier eh O(1)...
  
  
  while(i <= n)
    {
      steps++; //vergleich
      
      steps += 2; //if, Indexzugriff
      if(arr[i] == 1) 
      {
      	steps += 3; 
        printf("%d\n", i);
        if((2*i+1) <= n)
        {
	          arr[2*i+1] = 1;
	          steps += 4;
        } 
        
        steps += 3;
        if((3*i+1) <= n)
        {
          arr[3*i+1] = 1;
          steps += 4;
        }
        
      }
    
    steps++; //Wertzuweisung
    i++;
    
  } //n*20 Steps für n Schleifendurchläufe
  steps += n; 
  
  
  steps++; //Freigabe von Speicherplatz
  free(arr); 
  
  printf("#%lu\t%lu\n", n, steps);
  
return EXIT_SUCCESS;
}
