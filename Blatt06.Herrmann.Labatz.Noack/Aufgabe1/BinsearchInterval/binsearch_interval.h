#ifndef BINSEARCH_INTERVAL_H
#define BINSEARCH_INTERVAL_H

#include <stdlib.h>

typedef double Basetype;

const Basetype *binsearch(const Basetype *array,size_t length,Basetype key);
const Basetype *binsearch_gt_leq(const Basetype *array,size_t length,
                                 Basetype key);
const Basetype *binsearch_sm_geq(const Basetype *array,size_t length,
                                 Basetype key);

typedef struct
{
  size_t i, j;
} Indexrange;

Indexrange binsearch_interval(const Basetype *array,size_t length,
                              Basetype low,Basetype high);

#endif
