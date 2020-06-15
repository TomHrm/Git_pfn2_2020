#include "simul_evolution.h"
#include <stdbool.h>
#include <assert.h>

#define DEBUG() printf("Debug: Line %d\n", __LINE__)
size_t simulation_step(int *population, size_t num_0, const int pop_size,
											 const double *prob_divide)
{

	int num_individuals[2] = {num_0, (pop_size - num_0)};
	for(int i = 0; i < pop_size; i++)
		{
			double dec = drand48(); //zufallszahl, ob zellteilung
			if(prob_divide[population[i]] >= dec)
				{
					/* da der Zugriff population[pop_size] verboten ist: */
					int dead = pop_size;
					while(dead == pop_size)
					{
						dead = (int)pop_size * drand48();
					} 
					if(population[dead] != population[i])
					{
						num_individuals[abs(population[i]-1)]--;
						num_individuals[abs(population[dead]-1)]++;
						/*
							Erläuterung: abs(popualtion[n]-1) = 0 für n = 1 | = 1 für n = 0
						*/
					}
					//individuum dead wird durch typ population[i] ersetzt
					population[dead] = population[i];
				}
				
		}
		assert(pop_size == num_individuals[0] + num_individuals[1]);
		
		return num_individuals[0];
}

/*
int main()
{
	size_t num[] = {10, 10};
	double prob[] = {0.2, 0.3};
	
	simulation_run(num, prob, 100, NULL); 
	return 0;
}*/


void simulation_run(const size_t *num_individuals_input,
										const double *prob_divide,
										size_t generations, FILE *logfp)
{
	
	
	size_t num_individuals[2] = 
		{num_individuals_input[0], num_individuals_input[1]};
		
	//soll später beschrieben werden
	
	const int pop_size = num_individuals[0]+num_individuals[1];
	int *population = malloc(sizeof *population * pop_size);
	assert(population != NULL); 

	for(int i = 0; i < num_individuals[0]; i++)
	{
		population[i] = 0;
	}
	for(int j = num_individuals[0]; j <= pop_size; j++)
	{
		population[j] = 1;
	}
	
	
	int k = 0;
	while(k < generations)
	{
		num_individuals[0] = simulation_step(population, num_individuals[0],
		pop_size, prob_divide);
		num_individuals[1] = pop_size - num_individuals[0];
		
		if(logfp != NULL)
		{
			fprintf(logfp, "%d\t%ld\t%ld\n", k, num_individuals[0], num_individuals[1]);
		}
																	 
		if(num_individuals[0]==0)
		{
			printf("fixed:1\tsteps:%d\n", k);
			break;
		}
		if(num_individuals[1]==0)
		{
			printf("fixed:0\tsteps:%d\n", k);
			break;
		}	
		k++;
	}
	if(num_individuals[1]!=0 && num_individuals[0]!=0)
		{
			printf("simulation stopped after %d steps (0:%ld, 1:%ld)\n",
			k, num_individuals[0], num_individuals[1]);
		}
}


