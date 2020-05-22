#include <stdlib.h>
#include <stdio.h>

int hanoimoves(n)
{
    
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "%s: falsche Argumentennummer.\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n;
    if (sscanf(argv[1], "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "%s: nicht positiv.\n", argv[0]);
        return EXIT_FAILURE;
    }

    
    return EXIT_SUCCESS;
}
