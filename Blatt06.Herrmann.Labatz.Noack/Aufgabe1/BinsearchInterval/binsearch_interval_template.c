#include <assert.h>
#include <math.h>
#include "binsearch_interval.h"

/*
   compare two double values. If their absolute difference is smaller
   than 10^{-16}, they are considered to be the same values. If the
   first value is smaller than the second, the function returns -1.
   If the first value is larger than the second, the function returns 1.
*/

static int basetype_compare(Basetype d1, Basetype d2)
{
  if (fabs(d1 - d2) < 1.0e-16)
  {
    return 0;
  }
  if (d1 > d2)
  {
    return 1;
  }
  return -1;
}

/* In the array with length elements return a pointer to element in array
   equal to key. If the key does not occur, return NULL. */

const Basetype *binsearch(const Basetype *array,size_t length,Basetype key)
{
  const Basetype *leftptr = array,
                 *rightptr = array + length - 1;

  while (leftptr <= rightptr)
  {
    const Basetype *midptr = leftptr + (size_t) (rightptr-leftptr)/2;
    if (basetype_compare(key,*midptr) < 0)
    {
      /* key < *midptr => search index range ending before midptr */
      rightptr = midptr - 1;
    } else
    {
      if (basetype_compare(key,*midptr) > 0)
      {
        /* key > *midptr => search index range beginning after midptr */
        leftptr = midptr + 1;
      } else
      {
        /* key == *midptr => found */
        return midptr;
      }
    }
  }
  /* Nothing found */
  return NULL;
}

/* In the array with length elements return a pointer to the largest element
   in array which is <= key.
   If this element occurs more than once, return a pointer to its rightmost
   occurrence. If no element <= key exists, then return NULL.  */

const Basetype *binsearch_gt_leq(const Basetype *array,size_t length,
                                 Basetype key)
{
}

/* In the array with length elements return a pointer to the smallest
   element in array which is >= key.
   If this element occurs more than once, return a pointer to its leftmost
   occurrence. If no element >= key in array exists, then return NULL.  */

const Basetype *binsearch_sm_geq(const Basetype *array,size_t length,
                                 Basetype key)
{
}

/* Determine index range in array of length elements.
   The index range containts all elements >= low and <= high. If no such
   element exists, then return value is the empty index range represented
   the left boundary 1 and the right boundary 0. Parameter low represents
   \(\ell\) and parameter high represents \(h\) in the description of the
   exercise. */

Indexrange binsearch_interval(const Basetype *array,size_t length,
                              Basetype low,Basetype high)
{
  Indexrange ir = {1,0};
  const Basetype *leftptr, *rightptr, *ptr;

  if (basetype_compare(low,high) > 0)
  {
    /* Frage 1: Welcher Fall tritt hier auf und wof"ur steht der
       return-Wert? */
    return ir;
  }
  leftptr = binsearch_sm_geq(array,length,low);
  if (leftptr == NULL)
  {
    /* Frage 2: Welcher Fall tritt hier auf? */
    assert (basetype_compare(low,array[length-1]) > 0);
    return ir;
  }
  /* Frage 3: Wieso erfolgt die folgende binaere Suche nach der rechten Grenze
              nicht im kompletten Array, sondern im Speicherbereich
              mit (length - (size_t) (leftptr - array)) Elementen, auf den
              leftptr verweist? */
  rightptr = binsearch_gt_leq(leftptr,length - (size_t) (leftptr - array),high);
  if (rightptr == NULL)
  {
    /* Frage 4: Welcher Fall tritt hier auf? */
    assert (basetype_compare(high,*leftptr) < 0);
    return ir;
  }
  /* Frage 5: Warum ist diese Assertion hier sinnvoll? */
  assert (leftptr == array || basetype_compare(*(leftptr-1),low) < 0);
  /* Frage 6: Warum ist diese assertion hier sinnvoll? */
  assert (rightptr == array + length - 1 ||
          basetype_compare(*(rightptr+1),high) > 0);
  for (ptr = leftptr; ptr <= rightptr; ptr++)
  {
    /* Frage 7: Warum ist diese Assertion hier sinnvoll? */
    assert(basetype_compare(*ptr,low) >= 0);
    /* Frage 8: Warum ist diese Assertion hier sinnvoll? */
    assert(basetype_compare(*ptr,high) <= 0);
  }
  ir.i = (size_t) (leftptr - array);
  ir.j = (size_t) (rightptr - array);
  return ir;
}
