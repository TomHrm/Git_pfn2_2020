#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "list_of_numbers.h"
#include "subsetsum.h"

static void usage(const char *progname, bool show_options)
{
  const char *optionsmsg =
#ifdef WITH_DP_BASED_METHOD
      "  -d\tuse dp-based method\n"
#endif
      "  -f <filename>\tspecify filename with elements of set S\n"
      "  -m\tuse memoization-based method\n"
      "  -r <int> <int>\tspecify range of values for which to solve the\n"
      "                \tsubset sum problem (default: 1 1000)\n"
      "  -h\tshow this usage message";
  fprintf(stderr,
          "Usage: %s [options]\n"
          "solve subset sum problem for numbers in given file.\n%s\n",
          progname, show_options ? optionsmsg : "Use -h for more information.");
}

typedef struct
{
#ifdef WITH_DP_BASED_METHOD
  bool use_dp_based_method;
#endif
  bool use_memo_based_method;
  const char *filename;
  unsigned long lower_bound, upper_bound;
} Options;

static Options *options_new(int argc, char *const *argv)
{
  int opt;
  Options *options = malloc(sizeof *options);
  bool haserr = false;

  assert(options != NULL);
#ifdef WITH_DP_BASED_METHOD
  options->use_dp_based_method = false;
#endif
  options->use_memo_based_method = false;
  options->lower_bound = 1;
  options->upper_bound = 1000;
  options->filename = NULL;
  while ((opt = getopt(argc, argv, "dfmlrh")) != -1)
  {
    switch ((char) opt)
    {
#ifdef WITH_DP_BASED_METHOD
      case 'd':
        options->use_dp_based_method = true;
        break;
#endif
      case 'm':
        options->use_memo_based_method = true;
        break;
      case 'f':
        if (optind > argc - 1)
        {
          fprintf(stderr,"%s: missing arguments to option -f\n",argv[0]);
          usage(argv[0], false);
          haserr = true;
        } else
        {
          options->filename = argv[optind++];
        }
        break;
      case 'r':
        if (optind + 1 > argc - 1)
        {
          fprintf(stderr,"%s: missing arguments to option -r\n",argv[0]);
          usage(argv[0], false);
          haserr = true;
        } else
        {
          long read_lower, read_upper;

          if (sscanf(argv[optind],"%ld",&read_lower) != 1 || read_lower < 1 ||
              sscanf(argv[optind+1],"%ld",&read_upper) != 1 ||
              read_lower > read_upper)
          {
            fprintf(stderr,"%s: two arguments to -r must be two integers,\n"
                           "with the first smaller or equal to the second\n",
                            argv[0]);
            usage(argv[0], false);
            haserr = true;
          } else
          {
            options->lower_bound = (unsigned long) read_lower;
            options->upper_bound = (unsigned long) read_upper;
          }
          optind += 2;
        }
        break;
      case 'h':
        usage(argv[0], true);
        haserr = true;
        break;
      default:
        usage(argv[0], false);
        haserr = true;
        break;
    }
  }
  if (!haserr && optind != argc)
  {
    fprintf(stderr, "%s: Error: superfluous argument\n",argv[0]);
    usage(argv[0], false);
    haserr = true;
  }
  if (!haserr && options->filename == NULL)
  {
    fprintf(stderr, "%s: Error: option -f is mandatory\n",argv[0]);
    usage(argv[0], false);
    haserr = true;
  }
  if (haserr)
  {
    free(options);
    return NULL;
  }
  return options;
}

static void options_delete(Options *options)
{
  if (options != NULL)
  {
    free(options);
  }
}

static unsigned long show_subset(const unsigned long *arr,bool *mark,
                                 size_t setsize)
{
  unsigned long idx, sum = 0;
  bool first = true;

  for (idx = 0; idx < setsize; idx++)
  {
     sum += mark[idx] ? arr[idx] : 0;
  }
  printf("%lu = ",sum);
  for (idx = 0; idx < setsize; idx++)
  {
    if (mark[idx])
    {
      printf("%s%lu",first ? "" : " + ",arr[idx]);
      first = false;
    }
  }
  printf("\n");
  return sum;
}

int main(int argc, char *argv[])
{
  Options *options = options_new(argc, (char *const *) argv);
  ListOfNumbers *list_of_numbers = NULL;
  bool haserr = false;

  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
  list_of_numbers = list_of_numbers_new(options->filename);
  if (list_of_numbers == NULL)
  {
    haserr = true;
  }
  if (!haserr)
  {
    unsigned long number;
    const size_t setsize = list_of_numbers_length(list_of_numbers);
    size_t idx;
    const long *arr = list_of_numbers_ref(list_of_numbers);
    bool *mark;

    for (idx = 0; idx < setsize; idx++)
    {
      assert(arr[idx] >= 0 && (idx == 0 || arr[idx-1] <= arr[idx]));
    }
    for (number = options->lower_bound; number <= options->upper_bound;
         number++)
    {
#ifdef WITH_DP_BASED_METHOD
      if (options->use_dp_based_method)
      {
        mark = subsetsum_dp((const unsigned long *) arr,setsize,number);
      } else
      {
#endif
        if (options->use_memo_based_method)
        {
          mark = subsetsum_memo((const unsigned long *) arr,setsize,number);
        } else
        {
          mark = subsetsum((const unsigned long *) arr, setsize, number);
        }
#ifdef WITH_DP_BASED_METHOD
      }
#endif
      if (mark != NULL)
      {
        const unsigned long this_sum = show_subset((const unsigned long *) arr,
                                                   mark,setsize);
        assert(this_sum == number);
        free(mark);
      } else
      {
        printf("no subset of sum %lu found\n",number);
      }
    }
    list_of_numbers_delete(list_of_numbers);
  }
  options_delete(options);
  return EXIT_SUCCESS;
}
