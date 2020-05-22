#include <stdlib.h>
#include <stdio.h>


void hanoi(int size, int start, int goal)
{
    if (size > 1)
    {
        hanoi(size - 1, start, 6 - start - goal);
        printf("(%d,%d) ", start, goal);
        hanoi(size - 1, 6 - start - goal, goal);

    } else {
        printf("(%d,%d) ", start, goal);
    }
}


int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "%s: Invalid number of arguments.\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n;
    if (sscanf(argv[1], "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "%s: Input is not a positive integer.\n", argv[0]);
        return EXIT_FAILURE;
    }

    hanoi(n, 1, 2);

    printf("\n");
    
    return EXIT_SUCCESS;
}
