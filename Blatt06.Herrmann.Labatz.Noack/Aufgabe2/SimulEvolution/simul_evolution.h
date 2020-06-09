#ifndef SIMUL_EVOLUTION_H
#define SIMUL_EVOLUTION_H
#include <stdlib.h>
#include <stdio.h>

/* run simulation of distribution of two kinds of species according
   to the following model parameters:
    num_indivduals[0]    initial number of occurrences of individuals of type 0
    num_indivduals[1]    initial number of occurrences of individuals of type 1
    prob_divide[0]       probability that an individual of type 0 divides
    prob_divide[1]       probability that an individual of type 1 divides
    generations          maximum number of simulated generations
    if logfp != NULL, a protocol is written to logfp
    The protocol reports the number of species of each types in each step
    of the simulation. */

void simulation_run(const size_t *num_individuals, const double *divide_prob,
                    size_t generations, FILE *logfp);

#endif
