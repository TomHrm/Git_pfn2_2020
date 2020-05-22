#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	int n;
	sscanf(argv[1],"%d", &n);
	int i = 0;
	int sum1 = 0;
	int sum2 = 0;
	while(i <= n)
	{
		sum1 = 2*sum1+1;
		sum2 = 3*sum2+1;
		printf("%d\t%d\n",sum1, sum2 );
		i++;
	}
	return(EXIT_SUCCESS);
}