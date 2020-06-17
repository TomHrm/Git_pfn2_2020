#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>  /* for fstat */
#include <sys/stat.h>   /* for fstat */
#include <unistd.h>     /* for fstat */
#include <fcntl.h>     /* for open */
#include "pfn_file_info.h"

struct PfNFileInfo
{
  void *contents;
  size_t size;
};

static int pfn_file_open(const char *progname,const char *filename,size_t *size)
{
  int fd;
  struct stat buf;

  fd = open(filename,O_RDONLY); /* open file for read, get filedes */
  if (fd == -1) /* check for error code */
  {
    if (progname != NULL)
    {
      fprintf(stderr,"%s: ",progname);
    }
    fprintf(stderr,"cannot open file \"%s\"\n",filename);
    return -1;
  }
  if (fstat(fd,&buf) == -1)  /* get status information of file */
  {
    if (progname != NULL)
    {
      fprintf(stderr,"%s: ",progname);
    }
    fprintf(stderr,"cannot access status of file %s\n",filename);
    return -2;
  }
  *size = buf.st_size; /* store file size in address of size */
  return fd;
}

PfNFileInfo *pfn_file_info_new(const char *progname,const char *filename)
{
  int fd;
  PfNFileInfo *pfn_file_info = (PfNFileInfo *) malloc(sizeof *pfn_file_info);

  assert(pfn_file_info != NULL);
  fd = pfn_file_open(progname,filename,&pfn_file_info->size);
  if (fd < 0)
  {
    free(pfn_file_info);
    return NULL;
  }
  if (pfn_file_info->size == 0)
  {
    pfn_file_info->contents = NULL;
  } else
  {
    pfn_file_info->contents = malloc(pfn_file_info->size);
    assert(pfn_file_info->contents != NULL);
    if (read(fd,pfn_file_info->contents,pfn_file_info->size) == -1)
    {
      fprintf(stderr,"%s(%s) failed\n",__func__,filename);
      exit(EXIT_FAILURE);
    }
  }
  close(fd);
  return pfn_file_info;
}

void pfn_file_info_delete(PfNFileInfo *pfn_file_info)
{
  if (pfn_file_info != NULL)
  {
    if (pfn_file_info->size > 0)
    {
      free(pfn_file_info->contents);
    }
    free(pfn_file_info);
  }
}

size_t pfn_file_info_size(const PfNFileInfo *pfn_file_info)
{
  assert(pfn_file_info != NULL);
  return pfn_file_info->size;
}

void *pfn_file_info_contents(const PfNFileInfo *pfn_file_info)
{
  assert(pfn_file_info != NULL);
  return pfn_file_info->contents;
}
