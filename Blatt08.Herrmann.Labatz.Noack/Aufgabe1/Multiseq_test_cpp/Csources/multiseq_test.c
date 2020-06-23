#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "multiseq.h"
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
    multiseq = multiseq_new(program_name,filename,filecontents,file_size);
    if (multiseq == NULL)
    {
      haserr = true;
    }
  }
  if (!haserr)
  {
    if (line_width < 0)
    {
      printf("filename\t%s\n",filename);
      printf("number of sequences\t%lu\n",multiseq_num_of_sequences(multiseq));
      printf("number of symbols in sequences\t%lu\n",
             multiseq_total_sequence_length(multiseq));
      printf("maximum length of sequences\t%lu\n",
             multiseq_maximum_sequence_length(multiseq));
    } else
    {
      multiseq_show(line_width,multiseq);
    }
  }
  multiseq_delete(multiseq);
  pfn_file_info_delete(file_info);
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
