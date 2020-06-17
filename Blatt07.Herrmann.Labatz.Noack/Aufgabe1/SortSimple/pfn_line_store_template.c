#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "pfn_line_store.h"

struct PfNLineStore
{
  size_t nextfree; /* number of lines */
  PfNLine *lines;  /* array with references to lines */
  char separator;
};

PfNLineStore *pfn_line_store_new(unsigned char *file_contents,
                                 size_t size,char sep)
{
  PfNLineStore *pfn_line_store;
  unsigned char *current_line = file_contents;
  size_t remain = size, allocated = 0;

  if (size == 0)
  {
    return NULL;
  }
  pfn_line_store = malloc(sizeof *pfn_line_store);
  assert(pfn_line_store != NULL);
  pfn_line_store->nextfree = 0;
  pfn_line_store->lines = NULL;
  pfn_line_store->separator = sep;
  while (true)
  {
    unsigned char *next_sep;

    assert(current_line + remain <= file_contents + size);
    next_sep = memchr(current_line,sep,remain);
    if (next_sep != NULL)
    {
      const size_t current_length = (size_t) (next_sep - current_line + 1);

      if (pfn_line_store->nextfree >= allocated)
      {
        allocated += (allocated * 0.2) + 1024;
        pfn_line_store->lines
          = realloc(pfn_line_store->lines,
                    allocated * sizeof *pfn_line_store->lines);
      }
      assert(current_length > 0);
      remain -= current_length;
      *next_sep = '\0';
      pfn_line_store->lines[pfn_line_store->nextfree++]
        = (PfNLine) current_line;
      current_line = next_sep + 1;
    } else
    {
      break;
    }
  }
  return pfn_line_store;
}

void pfn_line_store_delete(PfNLineStore *pfn_line_store)
{
  if (pfn_line_store != NULL)
  {
    free(pfn_line_store->lines);
    free(pfn_line_store);
  }
}

size_t pfn_line_store_number(const PfNLineStore *pfn_line_store)
{
  assert(pfn_line_store != NULL && pfn_line_store->nextfree > 0);
  return pfn_line_store->nextfree;
}

PfNLine pfn_line_store_access(const PfNLineStore *pfn_line_store,size_t idx)
{
  assert(pfn_line_store != NULL && pfn_line_store->nextfree > 0);
  if (idx > pfn_line_store->nextfree - 1)
  {
    fprintf(stderr,"%s: index %lu is not in range [0,%lu]\n",__func__,idx,
            pfn_line_store->nextfree - 1);
    exit(EXIT_FAILURE);
  }
  return pfn_line_store->lines[idx];
}

char pfn_line_store_sep(const PfNLineStore *pfn_line_store)
{
  assert(pfn_line_store != NULL);
  return pfn_line_store->separator;
}

void pfn_line_store_show(const PfNLineStore *pfn_line_store)
{
  size_t idx, num_lines = pfn_line_store_number(pfn_line_store);

  for (idx = 0; idx < num_lines; idx++)
  {
    PfNLine line = pfn_line_store_access(pfn_line_store,idx);

    printf("%s%c",line,pfn_line_store->separator);
  }
}

void pfn_line_store_sort(PfNLineStore *pfn_line_store,CompareFunc compar)
{
  /* to be implemented for the SortSimple exercise */
}
