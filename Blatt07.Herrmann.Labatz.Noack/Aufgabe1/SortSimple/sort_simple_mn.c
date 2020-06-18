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
typedef struct
{
  bool numerical_order;
  bool reverse_order;
  unsigned long num_of_files;
  const char * const *filenames;
}Options;

void usage(bool showOptions, char *filename)
{
  fprintf(stderr, "Usage: %s [options] filename\n", filename);
  if(showOptions)
  {
    fprintf(stderr, "Sort lines of the named file and write the result to the standard output.\n"
    "-r	sort in reverse order\n"
    "-n	use numerical order\n"
    "-h	show this usage message\n"); 
  } else 
    {
      fprintf(stderr,"Use -h for more information.\n");
    }
}

Options *options_new(int argc, char* const* argv)
{ 
  bool haserr = false;
  int opt;
  Options *options = malloc(sizeof *options);
  assert(options != NULL);
  options -> reverse_order = false;
  options -> numerical_order = false;
  while ((opt = getopt(argc, argv, "rnh")) != -1)
  {
    switch((char)opt)
    {
      case'r':
        options->reverse_order = true;
        break;
      case 'n':
        options -> numerical_order = true;
        break;
      case 'h' :
        usage(true, argv[0]);
        break;
      default:
        assert((char) opt == '?');
        usage(false, argv[0]);
        haserr = true;
        break;
     }
   }
   
  if (!haserr)
  {
   
    if (optind < argc)
    {
      options->filenames = (const char * const *) (argv + optind);
      options->num_of_files = (unsigned long) (argc - optind);
    } else
    {
      fprintf(stderr,"%s: Error: filenames expected\n",argv[0]);
      usage(argv[0], false);
      haserr = true;
    }
  }
  if (haserr)
  {
    free(options);
    return NULL;
  }
  return options;
}

void options_delete(Options *options)
{
  if (options != NULL)
  {
    free(options);
  }
}

int main(int argc, char *argv[])
{
  Options *options = options_new(argc, (char *const *) argv);
  unsigned long idx;
  bool haserr = false;
	debug(" ");
  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
	debug(" ");
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
debug(" ");
      line_store =
          pfn_line_store_new((unsigned char *) file_contents, file_size, '\n');
debug(" ");
      if (line_store == NULL)
      {
        haserr = true;
      } else
      {
debug(" ");
        sort_simple(line_store, options->numerical_order,
                    options->reverse_order);
debug(" ");
        pfn_line_store_show(line_store);
debug(" ");
        pfn_line_store_delete(line_store);
      }
    }
debug(" ");
    pfn_file_info_delete(file_info);
  }
  options_delete(options);
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
