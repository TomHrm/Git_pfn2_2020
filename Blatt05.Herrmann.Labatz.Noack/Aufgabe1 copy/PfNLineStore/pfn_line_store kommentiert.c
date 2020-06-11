#include <stdio.h>
#include <stddef.h>
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
    PfNLineStore *pfnlinestore_obj; //deklarierung eines eigenen structs welches am ende ausgegeben werden soll
    pfnlinestore_obj = malloc(sizeof (*pfnlinestore_obj)); //Speicherzuweisung dafür

	if (pfnlinestore_obj == NULL) //prüfen ob das nicht null ist
	{
		fprintf(stderr, "./pfn_line_store.c: malloc %lu failed.\n", 
			sizeof(pfnlinestore_obj));
		exit(EXIT_FAILURE);
	}

    pfnlinestore_obj -> nextfree = 0; //die zeilen unseres stucts auf 0 weil wir erst anfangen also es am anfang immer 0 sein muss
    pfnlinestore_obj -> lines = NULL; //
    size_t num_of_lines = 0; //Nutzen wir zur Speicherberechnung
    int lineidx = 0; //nutzen wir als index für die Position des zuletzt eingelesen Linebreaks

	for (int i = 0; i < size ; i++) //for schleife so groß wie size ist
	{
		if (file_contents[i] == sep) //wenn an der Stelle i ein seperator ist dann
        {
            file_contents[i] = '\0'; //setzte diese Stelle i gleich unseren 0 terminerten
            if(num_of_lines <= pfnlinestore_obj -> nextfree) //wenn unsere speichervariable 
			//kleiner gleich unserer Zeilen ist also solange wie das dokument nicht zuende ist
            {
                num_of_lines += (num_of_lines * 0.2) + 1; //Berechnung, das +1 ist nur da damit wir irgendwas rauf rechnen sonst rechnen wir immer 0 * 0.2
                pfnlinestore_obj -> lines = realloc(pfnlinestore_obj ->lines, num_of_lines * (sizeof *pfnlinestore_obj -> lines));  
				// reallocieren des speichers für lines unseres Structs mittwels dereferenzierten sizeof                 
                assert(pfnlinestore_obj != NULL); //test ob Null, kann ansich weg
            }
		    pfnlinestore_obj -> lines[pfnlinestore_obj -> nextfree] = (PfNLine) &file_contents[lineidx];
			//lines soll überschrieben werden an der stelle nextfree unseres structs mit dem letzen linebreak
            lineidx = i+1; //linebreak + 1 um auf die neue Zeile zu kommen 
            pfnlinestore_obj -> nextfree++; //in die nächste zeile gehen
		}	
	}
	pfnlinestore_obj -> separator = sep; //unser Septerator der ausgegeben werden soll ist gleich dem normalen seperator

	return pfnlinestore_obj; //return unseres objects
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



