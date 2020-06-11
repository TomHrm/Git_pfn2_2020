#ifndef PFN_FILE_INFO_H
#define PFN_FILE_INFO_H
#include <stdlib.h>

typedef struct PfNFileInfo PfNFileInfo;

PfNFileInfo *pfn_file_info_new(const char *progname,const char *filename);

void pfn_file_info_delete(PfNFileInfo *pfn_file_info);

size_t pfn_file_info_size(const PfNFileInfo *pfn_file_info);

void *pfn_file_info_contents(const PfNFileInfo *pfn_file_info);

#endif
