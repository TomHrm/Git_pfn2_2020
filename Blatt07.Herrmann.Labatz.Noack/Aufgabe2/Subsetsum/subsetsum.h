#ifndef SUBSETSUM_H
#define SUBSETSUM_H
#include <stdbool.h>
/* Returns pointer mark != NULL if the ordered set of numbers in 
   <arr> of size <r> contains a
   subset with the sum of its members equal to <s>.
   In this case , <mark[i]> is true iff the <i>th
   element in <arr> is in the subset.
   If there is no subset of the elements in <arr>, then a NULL-pointer is
   returned.
 */

bool *subsetsum(const unsigned long *arr,
                unsigned long r,
                unsigned long s);

/* The following function uses the memoization technique to compute the
   same result as the previous function, but with much less rekursive
   calls. */

bool *subsetsum_memo(const unsigned long *arr,
                     unsigned long r,
                     unsigned long s);

#ifdef WITH_DP_BASED_METHOD
/* It is not necessary to implement this function. */

bool *subsetsum_dp(const unsigned long *arr,
                   unsigned long r,
                   unsigned long s);

#endif
#endif
