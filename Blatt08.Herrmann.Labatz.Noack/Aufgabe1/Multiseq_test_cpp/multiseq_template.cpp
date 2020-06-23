#include <ctype.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <stdexcept>
#include "multiseq.hpp"

/* This is a function like printf and fprintf which delivers a std::string
   object according to the specified format string <fmt> and the variable
   number of arguments. */

static std::string str_format(const char* fmt, ...)
{
  va_list vl;

  va_start(vl, fmt);
  int size = vsnprintf(0, 0, fmt, vl) + sizeof('\0');
  va_end(vl);

  char *buffer = new char [size];

  va_start(vl, fmt);
  size = vsnprintf(buffer, size, fmt, vl);
  va_end(vl);
  std::string ret = std::string(buffer, size);
  delete[] buffer;
  return ret;
}

static void throw_empty_sequence(const char *filename,
                                 unsigned long num_of_sequences)
{
  std::string msg;
  if (filename != NULL)
  {
    msg = str_format("file \"%s\": sequence number %lu is empty",
                     filename,num_of_sequences);
  } else
  {
    msg = str_format("input: sequence number %lu is empty",num_of_sequences);
  }
  throw std::invalid_argument(msg);
}

static void throw_missing_header(const char *filename)
{
  std::string msg;
  if (filename != NULL)
  {
    msg = str_format("file \"%s\": missing header for sequence 1",filename);
  } else
  {
    msg = str_format("input: missing header for sequence 1");
  }
  throw std::invalid_argument(msg);
}

/* now the implementation of the methods for class Multiseq follows. Here
   is the header of the fist method, which is the constructor. Note
   that the name of the program is not required as a parameter (as in the 
   C-Implementation), because the exception only generates the error message
   without the program-name itself. The latter is combined with the program
   name in the catch-block appearing in the main-function, where the
   program name is available in argv[0]. */
Multiseq::Multiseq(const char *filename,unsigned char *filecontents,
                   unsigned long file_size)
}

Multiseq::~Multiseq(void)
{
  // nothing to be done
}

unsigned long Multiseq::num_of_sequences(void) const
{
  // to be implemented
}

unsigned long Multiseq::maximum_sequence_length(void) const
{
  // to be implemented
}

unsigned long Multiseq::total_sequence_length(void) const
{
  // to be implemented
}

unsigned long Multiseq::header_length(unsigned long seqnum) const
{
  // to be implemented
}

unsigned long Multiseq::sequence_length(unsigned long seqnum) const
{
  // to be implemented
}

const unsigned char * Multiseq::sequence_ptr(unsigned long seqnum) const
{
  // to be implemented
}

const unsigned char * Multiseq::header_ptr(unsigned long seqnum) const
{
  // to be implemented
}

void Multiseq::show(unsigned long width) const
{
  // to be implemented
}
