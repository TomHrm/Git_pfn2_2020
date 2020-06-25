#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdbool>
#include <cassert>
#include "multiseq.hpp"
#include "pfn_file_info.h"

int main(int argc,char **argv)
{
  size_t file_size;
  long line_width;
  const char *filename, *program_name;
  PfNFileInfo *file_info = NULL;
  void *filecontents;
  Multiseq *multiseq = NULL;
  bool haserr = false;

  if (argc != 3 || sscanf(argv[1],"%ld",&line_width) != 1)
  {
    fprintf(stderr,"Usage:: %s <width> <fastafile>\n",argv[0]);
    return EXIT_FAILURE;
  }
  program_name = argv[0];
  filename = argv[2];
  file_info = pfn_file_info_new(program_name,filename);
  if (file_info == NULL)
  {
    return EXIT_FAILURE;
  }
  file_size = pfn_file_info_size(file_info);
  if (file_size == 0)
  {
    fprintf(stderr,"%s: file \"%s\" is empty\n",program_name,filename);
    haserr = true;
  }
  if (!haserr)
  {
    filecontents = pfn_file_info_contents(file_info);
  
    try
    {
      multiseq = new Multiseq(filename,(unsigned char *)filecontents,file_size);
    }
    catch (int e)
    {
      std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
  }
  if (!haserr)
  {
    if (line_width < 0)
    {
      std::cout << "filename\t" << filename << "\n";
      std::cout << "number of sequences\t" << multiseq -> num_of_sequences() << std :: endl;
      std::cout << "number of symbols in sequences\t" << multiseq -> total_sequence_length() << std :: endl;
      std::cout << "maximum length of sequences\t"<< multiseq -> maximum_sequence_length() << std :: endl;
    } else
    {
      multiseq -> show(line_width);
    }
  }
  multiseq -> ~Multiseq();
  pfn_file_info_delete(file_info);
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
