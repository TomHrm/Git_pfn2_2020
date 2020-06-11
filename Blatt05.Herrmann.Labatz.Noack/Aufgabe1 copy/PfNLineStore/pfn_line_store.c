#include <stdio.h>
#include "pfn_line_store.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct PfNLineStore
{
	size_t nextfree; 
	PfNLine *lines;
	char separator;
};

PfNLineStore *pfn_line_store_new(unsigned char *file_contents, size_t size, 
	char sep)
{
	char *array = malloc(size);
	size_t num_of_lines = 1;
	PfNLineStore *pfnlinestore_obj = NULL;
	if (file_contents == NULL)
	{
		fprintf(stderr, "./pfn_line_store.c: File %s not found.\n", 
			file_contents);
		exit(EXIT_FAILURE);
	}

	array[num_of_lines] = file_contents[0];
	for (int i = 0; i < size; i++)
	{
		if (file_contents[i] == sep)
		{
			num_of_lines++;
			array = realloc(array, sizeof (size) * (num_of_lines + 1));

			if ((i+1) < strlen((const char *) file_contents))
			{
				array[num_of_lines] = file_contents[i+1];
			}

			//file_contents[i] = (unsigned char) "\0";
		}	
	}

		/*if (file_contents[strlen((const char *) file_contents)] != sep)
		{
			num_of_lines++;
		}*/
	num_of_lines++;

	pfnlinestore_obj->nextfree = num_of_lines;
	pfnlinestore_obj->separator = sep;
	pfnlinestore_obj->lines = (const char **) array;
	printf("%zu\n", num_of_lines);
	return pfnlinestore_obj;
}

void pfn_line_store_delete(PfNLineStore *pfn_line_store)
{
	assert(pfn_line_store != NULL);

	free(pfn_line_store);
}

size_t pfn_line_store_number(const PfNLineStore *pfn_line_store)
{
	assert(pfn_line_store != NULL);

	return pfn_line_store->nextfree;
}

PfNLine pfn_line_store_access(const PfNLineStore *pfn_line_store, size_t i)
{
	assert(pfn_line_store != NULL);

	if (i > pfn_line_store->nextfree)
	{
		fprintf(stderr, "./pfn_line_store.c: Argument invalid.\n");
		exit(EXIT_FAILURE);
	}

	return pfn_line_store->lines[i];
}

char pfn_line_store_sep(const PfNLineStore *pfn_line_store)
{
	assert(pfn_line_store != NULL);

	return pfn_line_store->separator;
}

void pfn_line_store_show(const PfNLineStore *pfn_line_store)
{
	assert(pfn_line_store != NULL);

	for (int i = 0; i < pfn_line_store->nextfree; i++)
	{
		printf("%s\n",pfn_line_store->lines[i]);
	}
}

