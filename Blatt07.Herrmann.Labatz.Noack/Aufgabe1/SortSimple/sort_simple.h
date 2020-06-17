#ifndef SORT_SIMPLE_H
#define SORT_SIMPLE_H
#include <stdbool.h>
#include "pfn_line_store.h"

void sort_simple(PfNLineStore *line_store,bool numerical_order,
                 bool reverse_order);

#endif
