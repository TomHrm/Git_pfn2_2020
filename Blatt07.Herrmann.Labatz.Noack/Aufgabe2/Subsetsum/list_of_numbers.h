#ifndef LIST_OF_NUMBERS_H
#define LIST_OF_NUMBERS_H

typedef struct ListOfNumbers ListOfNumbers;

ListOfNumbers *list_of_numbers_new(const char *filename);
void list_of_numbers_delete(ListOfNumbers *list_of_numbers);
size_t list_of_numbers_length(const ListOfNumbers *list_of_numbers);
long list_of_numbers_get(const ListOfNumbers *list_of_numbers, size_t idx);
const long *list_of_numbers_ref(const ListOfNumbers *list_of_numbers);

#endif
