#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "array2dim.h"

typedef struct
{
  bool defined;
  bool has_sol;

} DefinedValues;

void print_array(const bool *b, unsigned long length)
{
  for (int i = 0; i < length; i++)
  {
    printf("%d: %d\n", i, b[i]);
  }
}

bool hs(const unsigned long *a, unsigned long i, unsigned long t, bool *mark)
{
  if (t == 0)
  {
    return true;
  } else if (i == 0)
  {
    return false;
  } else if (t >= a[i - 1] && hs(a, i - 1, t - a[i - 1], mark))
  {
    mark[i - 1] = true;
    return true;
  } else
  {
    mark[i - 1] = false;
    return hs(a, i - 1, t, mark);
  }
}

bool *subsetsum(const unsigned long *arr, unsigned long r, unsigned long s)
{
  bool *mark = calloc(r, sizeof *mark);

  if (!hs(arr, r, s, mark))
  {
    free(mark);
    return NULL;
  } else
  {
    return mark;
  }
}

bool hs_mat(const unsigned long *a, unsigned long i, unsigned long t,
            DefinedValues **val_mat, bool *mark)
{
  if (val_mat[i][t].defined)
  {
    return val_mat[i][t].has_sol;
  }
  if (t == 0)
  {
    return true;
  } else if (i == 0)
  {
    return false;
  } else if (t >= a[i - 1] && hs_mat(a, i - 1, t - a[i - 1], val_mat, mark))
  {
    val_mat[i][t].defined = true;
    val_mat[i][t].has_sol = true;
    mark[i - 1] = true;

    return true;
  } else
  {
    val_mat[i][t].defined = true;
    val_mat[i][t].has_sol = false;
    return hs_mat(a, i - 1, t, val_mat, mark);
  }
}

bool *subsetsum_memo(const unsigned long *arr, unsigned long r, unsigned long s)
{
  bool *mark = calloc(r, sizeof *mark);
  DefinedValues **val_mat;
  array2dim_malloc(val_mat, DefinedValues, r + 1, s + 1);
  for (int i = 0; i <= r; i++)
  {
    for (int j = 0; j <= s; j++)
    {
      val_mat[i][j].defined = false;
      val_mat[i][j].has_sol = false;
    }
  }

  if (!hs_mat(arr, r, s, val_mat, mark))
  {
    array2dim_delete(val_mat);
    return NULL;
  } else
  {
    return mark;
  }
}
