#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "list_of_numbers.h"

struct ListOfNumbers
{
  long *numbers;
  size_t nextfree;
};

ListOfNumbers *list_of_numbers_new(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  size_t allocated = 0;
  long number;
  ListOfNumbers *list_of_numbers;

  if (fp == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  list_of_numbers = malloc(sizeof *list_of_numbers);
  assert(list_of_numbers != NULL);
  list_of_numbers->numbers = NULL;
  list_of_numbers->nextfree = 0;
  while (fscanf(fp, "%ld", &number) == 1)
  {
    if (list_of_numbers->nextfree >= allocated)
    {
      allocated += (allocated * 0.2) + 4;
      list_of_numbers->numbers =
          realloc(list_of_numbers->numbers,
                  sizeof *list_of_numbers->numbers * allocated);
      assert(list_of_numbers->numbers != NULL);
    }
    assert(list_of_numbers->nextfree < allocated);
    list_of_numbers->numbers[list_of_numbers->nextfree++] = number;
  }
  fclose(fp);
  return list_of_numbers;
}

void list_of_numbers_delete(ListOfNumbers *list_of_numbers)
{
  if (list_of_numbers != NULL)
  {
    free(list_of_numbers->numbers);
    free(list_of_numbers);
  }
}

size_t list_of_numbers_length(const ListOfNumbers *list_of_numbers)
{
  assert(list_of_numbers != NULL);
  return list_of_numbers->nextfree;
}

long list_of_numbers_get(const ListOfNumbers *list_of_numbers, size_t idx)
{
  assert(list_of_numbers != NULL && idx < list_of_numbers->nextfree);
  return list_of_numbers->numbers[idx];
}

const long *list_of_numbers_ref(const ListOfNumbers *list_of_numbers)
{
  assert(list_of_numbers != NULL);
  return list_of_numbers->numbers;
}
