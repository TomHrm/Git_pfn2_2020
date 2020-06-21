#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include "simul_evolution.h"

static void usage(const char *progname, bool show_options,
                  size_t default_num_individuals, double default_prob,
                  size_t default_generations)
{
  fprintf(stderr,
          "Usage: %s [options]\n"
          "perform simulation of two types of individuals according to model\n"
          "parameters.\n",
          progname);
  if (show_options)
  {
    fprintf(
        stderr,
        "  -m <int>\tinitial number of occurrences of individuals of type 0\n"
        "          \t(default: %lu)\n"
        "  -n <int>\tinitial number of occurrences of individuals of type 1\n"
        "          \t(default: %lu)\n"
        "  -p <double>\tprobability that an individual of type 0 divides\n"
        "             \t(default: %.1f)\n"
        "  -q <double>\tprobability that an individual of type 1 divides\n"
        "             \t(default: %.2f)\n"
        "  -g <int>\tmaximum number of simulated generations (default: %lu)\n"
        "  -l <filename>\tspecify name of log file to store simulation state\n"
        "               \t(default: undefined)\n"
        "  -s <int>\tseed for the random number generator\n"
        "              \t(default: return value of time(NULL))\n"
        "  -h          \tshow this usage message\n",
        default_num_individuals, default_num_individuals, default_prob,
        default_prob, default_generations);
  } else
  {
    fprintf(stderr, "Use -h for more information.\n");
  }
}

typedef struct
{
  size_t num_indivduals[2], generations;
  long seed;
  double prob_divide[2];
  const char *logfilename;
} Options;

static Options *options_new(int argc, char *const *argv)
{
  int opt;
  bool haserr = false;
  const size_t default_num_individuals = 1000, default_generations = 50000;
  const double default_prob = 0.5;
  Options *options = malloc(sizeof *options);

  assert(options != NULL);
  options->num_indivduals[0] = options->num_indivduals[1] =
      default_num_individuals;
  options->prob_divide[0] = options->prob_divide[1] = default_prob;
  options->seed = 0;
  options->generations = default_generations;
  options->logfilename = NULL;
  while ((opt = getopt(argc, argv, "glmnpqsh")) != -1)
  {
    double read_double;
    long read_long;

    switch ((char) opt)
    {
      case 'p':
      case 'q':
        if (optind > argc - 1)
        {
          fprintf(stderr, "%s: Error: missing parameter to option -%c\n",
                  argv[0], (char) opt);
          haserr = true;
          break;
        }
        if (sscanf(argv[optind], "%lf", &read_double) != 1 ||
            read_double < 0.0 || read_double > 1.0)
        {
          fprintf(stderr,
                  "%s: Error: argument to option -%c must be floating "
                  "point value in the range from 0.0 to 1.0\n",
                  argv[0], (char) opt);
          haserr = true;
          break;
        }
        options->prob_divide[opt == 'p' ? 0 : 1] = read_double;
        optind++;
        break;
      case 'g':
      case 's':
      case 'm':
      case 'n':
        if (optind > argc - 1)
        {
          fprintf(stderr, "%s: Error: missing parameter to option -%c\n",
                  argv[0], (char) opt);
          haserr = true;
          break;
        }
        if (sscanf(argv[optind], "%ld", &read_long) != 1 || read_long < 1)
        {
          fprintf(stderr,
                  "%s: Error: argument to option -%c must be "
                  "positive integer\n",
                  argv[0], (char) opt);
          haserr = true;
          break;
        }
        if (opt == 'g')
        {
          options->generations = (size_t) read_long;
        } else
        {
          if (opt == 's')
          {
            options->seed = read_long;
          } else
          {
            assert(opt == 'm' || opt == 'n');
            options->num_indivduals[opt == 'm' ? 0 : 1] = (size_t) read_long;
          }
        }
        optind++;
        break;
      case 'l':
        if (optind > argc - 1)
        {
          fprintf(stderr, "%s: Error: missing parameter to option -%c\n",
                  argv[0], (char) opt);
          haserr = true;
          break;
        }
        options->logfilename = argv[optind];
        optind++;
        break;
      case 'h':
        usage(argv[0], true, default_num_individuals, default_prob,
              default_generations);
        haserr = true;
        break;
      default:
        assert((char) opt == '?');
        usage(argv[0], false, default_num_individuals, default_prob,
              default_generations);
        haserr = true;
        break;
    }
  }
  if (!haserr)
  {
    if (optind < argc)
    {
      fprintf(stderr, "%s: superfluous argument %s\n", argv[0], argv[optind]);
      usage(argv[0], false, default_num_individuals, default_prob,
            default_generations);
      haserr = true;
    } else
    {
      assert(optind == argc);
    }
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

int main(int argc, char *argv[])
{
  FILE *logfp = NULL;
  bool haserr = false;
  Options *options = options_new(argc, (char *const *) argv);

  if (options == NULL)
  {
    return EXIT_FAILURE;
  }
  if (options->logfilename != NULL)
  {
    logfp = fopen(options->logfilename, "w");
    if (logfp == NULL)
    {
      fprintf(stderr, "%s: cannot open logfile %s\n", argv[0],
              options->logfilename);
      haserr = true;
    }
  }
  if (!haserr)
  {
    long lseed;

    if (options->seed == 0)
    {
      lseed = (long) time(NULL);
    } else
    {
      lseed = options->seed;
    }
    fprintf(stderr, "# seed %ld\n", lseed);
    srand48(lseed);
    simulation_run(options->num_indivduals, options->prob_divide,
                   options->generations, logfp);
  }
  if (logfp != NULL)
  {
    fclose(logfp);
  }
  options_delete(options);
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
