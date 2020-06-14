#include <stdlib.h>
#include <stdio.h>
#include "pfn_line_store.h"
#include <stddef.h>
#include <assert.h>
#include <string.h>

struct PfNLineStore
{
    size_t nextfree; /* number of lines */
    PfNLine *lines; /* array with references to lines */ 
    char separator;
};

PfNLineStore *pfn_line_store_new(unsigned char *file_contents, size_t size,
                                  char sep)
{
  PfNLineStore * result;
  result = malloc(sizeof(*result));

  if(result == NULL)
  {
    fprintf(stderr, "%s: malloc(%lu) failed",__FILE__,sizeof(PfNLineStore));
    exit(EXIT_FAILURE);
  }
  
  result -> nextfree = 0;
  result -> lines = NULL;
  size_t allocated = 0;
  unsigned int linestartidx = 0; //Position des zuletzt einelgesenen Linebreak
  for(unsigned int i = 0; i < size; i++)
  {
    if(file_contents[i] == sep)
    {
    file_contents[i] = '\0';
    //result -> lines[counter+1] = (PfNLine) nextline;
    if(allocated <= result -> nextfree)
    {
      allocated += (allocated * 0.2)+ 32;
      result->lines = realloc(result->lines, allocated*(sizeof *result->lines));
      assert(result != NULL);
    }
    result ->lines[result -> nextfree] = (PfNLine) &file_contents[linestartidx];
    linestartidx = i+1;
    result -> nextfree++;
    }
  }
  result -> separator = sep;
  return result;
}

void pfn_line_store_delete(PfNLineStore *pfn_line_store)
{
    if(pfn_line_store != NULL)
    {
        if(pfn_line_store -> lines != NULL)
        {
          free(pfn_line_store -> lines);
        }
        free(pfn_line_store);
    }
}
size_t pfn_line_store_number(const PfNLineStore *pfn_line_store)
{
  assert(pfn_line_store != 0);
  return(pfn_line_store->nextfree);
}

PfNLine pfn_line_store_access(const PfNLineStore *pfn_line_store,size_t i)
{
  assert(pfn_line_store != 0);
  return(pfn_line_store->lines[i]);
}

char pfn_line_store_sep(const PfNLineStore *pfn_line_store)
{
  assert(pfn_line_store != 0);
  return(pfn_line_store->separator);
}

void pfn_line_store_show(const PfNLineStore *pfn_line_store)
{
  assert(pfn_line_store != 0);
  for(int n = 0; n < pfn_line_store->nextfree;n++)
  {
    printf("%s\n",pfn_line_store->lines[n]);
  }
}
