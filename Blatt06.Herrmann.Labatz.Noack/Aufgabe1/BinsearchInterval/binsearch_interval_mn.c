#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "binsearch_interval.h"

static void usage(const char *progname, const char *msg, bool show_options,
                  unsigned long default_length, unsigned long default_searches,
                  double default_expected_distance,
                  double default_expected_interval_width)
{
  if (msg != NULL)
  {
    fprintf(stderr, "%s: %s\n", progname, msg);
  }
  fprintf(stderr, "Usage: %s [options]\n", progname);
  if (show_options)
  {
    fprintf(stderr,
            "run interval search on sorted array\n"
            "  -l\tlength of array (default: %lu)\n"
            "  -s\tnumber of searches (default: %lu)\n"
            "  -d\texpected distance of values in array (default: %.2f)\n"
            "  -i\texpected width of interval (default: %.2f)\n"
            "  -h\tshow this usage message\n",
            default_length, default_searches, default_expected_distance,
            default_expected_interval_width);
  } else
  {
    fprintf(stderr, "Use -h for more information.\n");
  }
}

typedef struct
{
  unsigned long length, searches;
  double expected_distance, expected_interval_width;
} MultiBinsearchOptions;

#define USAGE(MSG, SHOW_OPTIONS)                                      \
  usage(argv[0], MSG, SHOW_OPTIONS, default_length, default_searches, \
        default_expected_distance, default_expected_interval_width)

#define POSITIVE_INTEGER_ARGUMENT(OPTION_CHAR, OPTION_CHAR_RAW, VAR) \
  case OPTION_CHAR:                                                  \
    if (sscanf(optarg, "%ld", &read_long) != 1 || read_long <= 0)    \
    {                                                                \
      USAGE("illegal argument to option -" #OPTION_CHAR_RAW          \
            ": must be positive integer",                            \
            true);                                                   \
      haserr = true;                                                 \
    } else                                                           \
    {                                                                \
      VAR = (unsigned long) read_long;                               \
    }                                                                \
    break

#define POSITIVE_FLOATING_POINT_ARGUMENT(OPTION_CHAR, OPTION_CHAR_RAW, VAR) \
  case OPTION_CHAR:                                                         \
    if (sscanf(optarg, "%lf", &read_double) != 1 || read_double <= 0)       \
    {                                                                       \
      USAGE("illegal argument to option -" #OPTION_CHAR_RAW                 \
            ": must be positive floating point value",                      \
            true);                                                          \
      haserr = true;                                                        \
    } else                                                                  \
    {                                                                       \
      VAR = read_double;                                                    \
    }                                                                       \
    break

static MultiBinsearchOptions *multi_binsearch_options_new(int argc,
                                                          char *const *argv)
{
  int opt;
  MultiBinsearchOptions *options = malloc(sizeof *options);
  bool haserr = false;
  long read_long;
  double read_double;
  const unsigned long default_length = 1000000, default_searches = 10000;
  const double default_expected_distance = 50.0,
               default_expected_interval_width = 500.0;

  assert(options != NULL);
  options->length = default_length;
  options->searches = default_searches;
  options->expected_distance = default_expected_distance;
  options->expected_interval_width = default_expected_interval_width;
  while ((opt = getopt(argc, argv, "l:s:d:i:h")) != -1)
  {
    switch ((char) opt)
    {
      POSITIVE_INTEGER_ARGUMENT('l', l, options->length);
      POSITIVE_INTEGER_ARGUMENT('s', s, options->searches);
      POSITIVE_FLOATING_POINT_ARGUMENT('d', d, options->expected_distance);
      POSITIVE_FLOATING_POINT_ARGUMENT('i', i,
                                       options->expected_interval_width);
      case 'h':
        USAGE(NULL, true);
        haserr = true;
        break;
      default:
        assert((char) opt == '?');
        USAGE(NULL, false);
        haserr = true;
        break;
    }
  }
  if (haserr)
  {
    free(options);
    return NULL;
  }
  return options;
}

static void multi_binsearch_options_delete(MultiBinsearchOptions *options)
{
  if (options != NULL)
  {
    free(options);
  }
}

static int compare_unsigned_long(const void *va, const void *vb)
{
  unsigned long a = *((const unsigned long *) va),
                b = *((const unsigned long *) vb);

  return (a < b) ? 1 : ((a > b) ? -1 : 0);
}

static void show_irange_sizes(unsigned long *irange_sizes, size_t num_values)
{
  if (num_values > 0)
  {
    unsigned long count = 1, idx;

    /* Frage A: In welcher Reihenfolge werden hier die Elemente
       sortiert und wodurch entsteht diese Reihenfolge?
                         Antwort A: Elemente werden nach absteigender
       Reihenfolge sortiert über die Funtion compare_unsigned_long()*/
    qsort(irange_sizes, num_values, sizeof *irange_sizes,
          compare_unsigned_long);
    /* Frage B: Was berechnet die folgende Schleife auf der Basis
       des sortierten Arrays irange_sizes?
       Antwort B: Es wird ausgegeben, wie häufig eine indexrange vorkommt*/
    for (idx = 0; idx < num_values - 1; idx++)
    {
      if (irange_sizes[idx] > irange_sizes[idx + 1])
      {
        printf("%lu\t%lu\n", irange_sizes[idx], count);
        count = 1;
      } else
      {
        assert(irange_sizes[idx] == irange_sizes[idx + 1]);
        count++;
      }
    }
    printf("%lu\t%lu\n", irange_sizes[num_values - 1], count);
  }
}

/* Frage C: Charakterisieren Sie die Werte im Array, das durch die
   Funktion generate_array erzeugt wird.
   Antwort C: Das Array beginnt bei 0 und wird durchschnittlich in jeder Zelle
   um expected_distance erh"ot. */
static double *generate_array(size_t length, double expected_distance)
{
  size_t idx;
  double current = 0.0;

  double *array = malloc(length * sizeof *array);
  assert(array != NULL && expected_distance > 0.0);
  srand48(366292341L); /* use some large prime number */
  for (idx = 0; idx < length; idx++)
  {
    array[idx] = current;
    current += expected_distance * 2.0 * drand48();
  }
  return array;
}

int main(int argc, char *argv[])
{
  unsigned long idx;
  size_t *irange_sizes;
  double *array, last_element;

  MultiBinsearchOptions *options =
      multi_binsearch_options_new(argc, (char *const *) argv);

  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
  array = generate_array(options->length, options->expected_distance);
  /*for(int i = 0; i < options -> length; i++)
  {
          printf("%d %.8f\n", i, array[i]);
  }*/
  irange_sizes = malloc(options->searches * sizeof *irange_sizes);
  assert(irange_sizes != NULL);
  last_element = array[options->length - 1];
  for (idx = 0; idx < options->searches; idx++)
  {
    /* Frage D: Was sind die Wertebereiche der Variablen low und high nach
                  Initialisierung durch die folgenden Anweisungen?
                Antwort: low \in [0, last_element]
                                                 high \in [low, low +
       2*expected_interval_width]
                */
    double low = last_element * drand48(),
           high = low + options->expected_interval_width * 2.0 * drand48();
    Indexrange itv = binsearch_interval(array, options->length, low, high);
    irange_sizes[idx] = (itv.i > itv.j) ? 0 : itv.j - itv.i + 1;
  }
  show_irange_sizes(irange_sizes, options->searches);
  free(array);
  free(irange_sizes);
  multi_binsearch_options_delete(options);
  return EXIT_SUCCESS;
}
