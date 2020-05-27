#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "pfn_file_info.h"
#include "pfn_line_store.h"

static void usage(const char *progname,bool show_options)
{
  const char *optionsmsg =
    "output number of lines and/or reverse lines of specified files\n"
    "  -l\toutput number of lines\n"
    "  -o\tshow lines in reverse order\n"
    "  -r\tshow characters of each line in reverse order\n"
    "  -h\tshow this usage message";
  fprintf(stderr, "Usage: %s [options] ... filename1 [filename2...]\n%s\n",
          progname,
          show_options ? optionsmsg : "Use -h for more information.");
}

/* FRAGE 1: Wozu dient die Struktur PfNLineStoreOptions? */

typedef struct
{
  bool show_num_lines,
       show_rev_each_line,
       show_reversed_order;
  unsigned long num_of_files;
  const char * const *filenames;
} PfNLineStoreOptions;

static PfNLineStoreOptions *pfn_line_store_options_new(int argc,
                                                       char *const *argv)
{
  int opt;
  PfNLineStoreOptions *options = malloc(sizeof *options);
  bool haserr = false;

  assert(options != NULL);
  options->show_num_lines = false;
  options->show_rev_each_line = false;
  options->show_reversed_order = false;
  /* FRAGE 2: Was passiert in der folgenden while-Schleife? Sie m"ussen
     hier keine technischen Details beschreiben, und es ist nicht notwendig,
     dass Sie das Manual zu getopt konsultieren. Die Antwort ergibt sich aus dem
     Kontext. */
  while ((opt = getopt(argc, argv, "lroh")) != -1)
  {
    switch ((char) opt)
    {
      case 'l':
        options->show_num_lines = true;
        break;
      case 'o':
        options->show_reversed_order = true;
        break;
      case 'r':
        options->show_rev_each_line = true;
        break;
      case 'h':
        usage(argv[0], true);
        /* FRAGE 3: Wozu wird die Variable haserr verwendet? */
        haserr = true;
        break;
      default:
        assert((char) opt == '?');
        usage(argv[0], false);
        haserr = true;
        break;
    }
  }
  if (!haserr)
  {
    /* FRAGE 4: Wo wird die Variable optind deklariert und
       welche Bedeutung hat sie? */
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

static void pfn_line_store_options_delete(PfNLineStoreOptions *options)
{
  if (options != NULL)
  {
    free(options);
  }
}

/* Diese Funktion gibt die Zeilen aus der PfNLineStore-Struktur
   wieder aus.
   FRAGE 5: Wie sieht in der folgenden Funktion die Ausgabe des
   Dateiinhalts in Abh"angigkeit der beiden boolschen-Parameter aus?
   FRAGE 6: Welche Abstraktion wird in der folgenden Funktion verwendet,
   damit man die "au"sere Schleife nur einmal schreiben muss?
*/

static void show_lines_generic(const PfNLineStore *line_store,
                               bool reversed_order,
                               bool rev_each_line)
{
  size_t idx, first_idx, last_idx,
         num_lines = pfn_line_store_number(line_store);
  int step;
  const char sep = pfn_line_store_sep(line_store);

  if (reversed_order)
  {
    first_idx = num_lines-1;
    last_idx = 0;
    step = -1;
  } else
  {
    first_idx = 0;
    last_idx = num_lines-1;
    step = 1;
  }
  for (idx = first_idx; /* nocheck */; idx += step)
  {
    const char *line_ptr = pfn_line_store_access(line_store,idx);
    const size_t line_length = strlen(line_ptr);

    if (rev_each_line)
    {
      const char *ptr = line_ptr + line_length - 1;

      for (/* Nothing */; ptr >= line_ptr; ptr--)
      {
        putchar(*ptr);
      }
    } else
    {
      /* write the memory of (line_length * sizeof *line_ptr) bytes referred
         to by line_ptr to stdout */
      fwrite(line_ptr,sizeof *line_ptr,line_length,stdout);
    }
    putchar(sep);
    if (idx == last_idx)
    {
      break;
    }
  }
}

int main(int argc,char *argv[])
{
  unsigned long idx;
  size_t total_lines = 0;
  PfNLineStoreOptions *options
    = pfn_line_store_options_new(argc,(char *const *) argv);
  bool haserr = false;

  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
  for (idx = 0; idx < options->num_of_files; idx++)
  {
    PfNFileInfo *file_info
      = pfn_file_info_new(argv[0],options->filenames[idx]);
    size_t file_size, num_lines = 0;

    if (file_info == NULL)
    {
      return EXIT_FAILURE;
    }
    file_size = pfn_file_info_size(file_info);
    if (file_size > 0)
    {
      PfNLineStore *line_store;
      void *file_contents = pfn_file_info_contents(file_info);

      line_store = pfn_line_store_new((unsigned char *) file_contents,
                                      file_size,'\n');
      if (line_store == NULL)
      {
        haserr = true;
      } else
      {
        num_lines = pfn_line_store_number(line_store);
        total_lines += num_lines;
        if (options->show_reversed_order || options->show_rev_each_line)
        {
          show_lines_generic(line_store,
                             options->show_reversed_order,
                             options->show_rev_each_line);
        }
      }
      pfn_line_store_delete(line_store);
    }
    if (!haserr && options->show_num_lines)
    {
      printf("%8lu %s\n",num_lines,options->filenames[idx]);
    }
    pfn_file_info_delete(file_info);
  }
  if (!haserr && options->show_num_lines)
  {
    printf("%8lu total\n",total_lines);
  }
  pfn_line_store_options_delete(options);
  return EXIT_SUCCESS;
}
