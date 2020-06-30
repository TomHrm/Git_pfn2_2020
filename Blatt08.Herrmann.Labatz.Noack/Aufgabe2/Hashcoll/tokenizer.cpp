#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <cassert>
#include "tokenizer.hpp"

#define CHECKBUFFERSIZE                                            \
  if (nextfree >= allocated)                                       \
  {                                                                \
    allocated += (allocated * 0.2) + 128;                          \
    buffer = (char *) realloc(buffer, sizeof *buffer * allocated); \
    assert(buffer != NULL);                                        \
  }

typedef void (*TokenHandlerFunc)(const char *, void *);

static void tokenizer(std::ifstream *infile, TokenHandlerFunc tokenhandler,
                      void *data)
{
  bool inword = false;
  unsigned long nextfree = 0, allocated = 0;
  char *buffer = NULL;

  while (infile->good())  // loop while extraction from file is possible
  {
    unsigned char cc = infile->get();  // get character from file

    if (isalpha(cc) || isdigit(cc) || cc == (int) '_')
    {
      CHECKBUFFERSIZE;
      buffer[nextfree++] = (char) cc;
      inword = true;
    } else
    {
      if (inword)
      {
        CHECKBUFFERSIZE;
        buffer[nextfree] = '\0';
        tokenhandler(buffer, data);
        nextfree = 0;
        inword = false;
      }
    }
  }
  if (inword)
  {
    CHECKBUFFERSIZE;
    buffer[nextfree] = '\0';
    tokenhandler(buffer, data);
  }
  free(buffer);
}

static void add2set(const char *s, void *data)
{
  str_set *words = (str_set *) data;

  (void) words->insert(s);
}

str_set *file2wordset(std::ifstream *infile)
{
  str_set *wordset = new str_set;
  tokenizer(infile, add2set, wordset);
  return wordset;
}
