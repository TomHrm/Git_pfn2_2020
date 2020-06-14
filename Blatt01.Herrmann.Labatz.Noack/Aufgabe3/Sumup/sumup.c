#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <regex.h>

unsigned long gaussSumme(unsigned long zahl) //Berechnet die Summe aller 
                                // natürlichen Zahlen von 0 bis @param zahl
{
	return ((zahl *(zahl +1))/2);
	//return 3 + zahl;
}

int main(int argc, char *argv[])
{
	long sign_m;
	long sign_n;
	if(argc != 3 || (sscanf(argv[1], "%ld", &sign_m) !=1 ) ||
	               (sscanf(argv[2], "%ld", &sign_n)) != 1 )
	{
		printf("Parameter-zahl oder form ungültig \n");
		exit(EXIT_FAILURE);
	}	
	if((sign_m < 0) || (sign_n < 0))
	{
		printf("negative Werte nicht erlaubt\n");
		exit(EXIT_FAILURE);
	}
	unsigned long m = sign_m;
	unsigned long n = sign_n;	
	unsigned long summe = 0;
	unsigned long summe_gauss = 0;
	
	if(m <= n)
		{
			//berechenung via  Summenformel
			for(int i = m; i < n+1; i++)
				{
					summe += i;
				}
				
			// Berechnung via Gauß
			summe_gauss = gaussSumme(n) - gaussSumme(m-1);
		}
		
	printf("%lu\t%lu\t%lu\n", m, n, summe);

	
	assert (summe == summe_gauss);
	return EXIT_SUCCESS;
}


