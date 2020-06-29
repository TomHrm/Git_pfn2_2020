#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include "multiseq.hpp"
#include "pfn_file_info.h"

int main(int argc, char **argv)
{
    size_t file_size;
    long line_width;
    const char *filename, *program_name;
    PfNFileInfo *file_info = NULL;
    Multiseq *multiseq;
    void *filecontents;
    bool haserr = false;

    if (argc != 3 || sscanf(argv[1], "%ld", &line_width) != 1)
    {
        std::cerr << "Usage:: " << argv[0] << " <width> <fastafile>\n"
                  << std::endl;
        return EXIT_FAILURE;
    }
    program_name = argv[0];
    filename = argv[2];
    file_info = pfn_file_info_new(program_name, filename);
    if (file_info == NULL)
    {
        return EXIT_FAILURE;
    }
    file_size = pfn_file_info_size(file_info);
    if (file_size == 0)
    {
        std::cerr << program_name << ": file \"" << filename << "\" is empty" << std::endl;
        haserr = true;
    }
    filecontents = pfn_file_info_contents(file_info);
    if (!haserr)
    {
        try
        {
            multiseq = new Multiseq(filename, (unsigned char *)filecontents, file_size);
        }
        catch (const std::invalid_argument &msg)
        {
            std::cerr << program_name << ": " << msg.what() << std::endl;
            haserr = true;
        }
    }
    if (!haserr)
    {
        if (line_width < 0)
        {
            printf("filename\t%s\n", filename);
            printf("number of sequences\t%lu\n", multiseq->num_of_sequences());
            printf("number of symbols in sequences\t%lu\n",
                   multiseq->total_sequence_length());
            printf("maximum length of sequences\t%lu\n",
                   multiseq->maximum_sequence_length());
        }
        else
        {
            multiseq->show(line_width);
        }
    }
    pfn_file_info_delete(file_info);
    return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
