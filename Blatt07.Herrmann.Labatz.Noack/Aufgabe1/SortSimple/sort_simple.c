#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pfn_line_store.h"
#include "sort_simple.h"

int lex_cmp(const void* line1, const void* line2)
{
  const PfNLine *l1 = (const PfNLine *) line1;
  const PfNLine *l2 = (const PfNLine *) line2;
  return -strcmp(*l1, *l2);
}

bool line_is_numeric(PfNLine line)
{
  for(int i = 0; i < strlen(line); i++)
  {
    if(!isdigit(line[i]) && line[i] != '.')
    {
      return false;
    }
  }
  return true;
}

int num_cmp(const void* line1, const void* line2)
{
  const PfNLine *l1 = (const PfNLine *) line1;
  const PfNLine *l2 = (const PfNLine *) line2;
  
  bool num1 = line_is_numeric(*l1);
  bool num2 = line_is_numeric(*l2);
  
  if(!(num1 || num2))
  {
    return lex_cmp(line1, line2);
  }
  double d1 = 0.0;
  double d2 = 0.0;

  if(num1)
  {
    if(!num2)
    {
      return 1;
    }
    d1 = atof(*l1);
  }
  if(num2)
  {
    if(!num1)
    {
        return -1;
    }
    d2 = atof(*l2);
  }
  return d1 >= d2? -1 : 1;
}

int lex_reverse_cmp(const void* line1, const void* line2)
{
  return -lex_cmp(line1, line2);
}

int num_reverse_cmp(const void* line1, const void* line2)
{
  return -num_cmp(line1, line2);
}

void sort_simple(PfNLineStore * line_store, bool numerical_order,
                                            bool reverse_order)
{
  if(numerical_order)
  {
    if(reverse_order)
    {
      pfn_line_store_sort(line_store, num_cmp);
    } else {
      pfn_line_store_sort(line_store, num_reverse_cmp);
    }
    
  } else
  {
    if(reverse_order)
    {
      pfn_line_store_sort(line_store, lex_cmp);
    } else {
      pfn_line_store_sort(line_store, lex_reverse_cmp);
    }
  }
}
