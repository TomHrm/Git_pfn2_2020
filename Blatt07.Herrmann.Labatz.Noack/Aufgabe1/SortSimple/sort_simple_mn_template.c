#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pfn_file_info.h"
#include "sort_simple.h"

/* Hier den code der Funktionen
   usage
   options_new
   options_delete
   und die Deklaration des Typs Options einf"ugen. */


int main(int argc, char *argv[])
{
  Options *options = options_new(argc, (char *const *) argv);
  unsigned long idx;
  bool haserr = false;

  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
  for (idx = 0; idx < options->num_of_files; idx++)
  {
    size_t file_size;
    PfNFileInfo *file_info =
        pfn_file_info_new(argv[0], options->filenames[idx]);

    if (file_info == NULL)
    {
      return EXIT_FAILURE;
    }
    file_size = pfn_file_info_size(file_info);
    if (file_size > 0)
    {
      PfNLineStore *line_store;
      void *file_contents = pfn_file_info_contents(file_info);

      line_store =
          pfn_line_store_new((unsigned char *) file_contents, file_size, '\n');
      if (line_store == NULL)
      {
        haserr = true;
      } else
      {
        sort_simple(line_store, options->numerical_order,
                    options->reverse_order);
        pfn_line_store_show(line_store);
        pfn_line_store_delete(line_store);
      }
    }
    pfn_file_info_delete(file_info);
  }
  options_delete(options);
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
