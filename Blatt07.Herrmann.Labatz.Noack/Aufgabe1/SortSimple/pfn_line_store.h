#ifndef PFN_LINE_STORE_H
#define PFN_LINE_STORE_H

#define debug(s) //printf("Debug: filename %s, line: %d, msg: %s\n", __FILE__, __LINE__, s)

typedef struct PfNLineStore PfNLineStore;

PfNLineStore *pfn_line_store_new(unsigned char *file_contents,
                                 size_t size, char sep);

void pfn_line_store_delete(PfNLineStore *pfn_line_store);

size_t pfn_line_store_number(const PfNLineStore *pfn_line_store);

typedef const char * PfNLine;

PfNLine pfn_line_store_access(const PfNLineStore *pfn_line_store,size_t idx);

void pfn_line_store_show(const PfNLineStore *pfn_line_store);

char pfn_line_store_sep(const PfNLineStore *pfn_line_store);

typedef int (*CompareFunc)(const void *,const void *);

void pfn_line_store_sort(PfNLineStore *pfn_line_store,CompareFunc compar);

#endif
