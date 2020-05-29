#include <stdio.h>
#include <stdlib.h>

static void hanoi_rec_count(int num_of_disks, int from_pole, int to_pole)
{
  if (num_of_disks > 0)
  {
    /* tmp_pole is the third peg for disk movement
       1 <= tmp_pole - from_pole - to_pole <= 3,
       1 <= from_pole,
       to_pole <= 3,
       from_pole != to_pole,
       We move all disks except the largest to the tmp_pole.
       Then we can move the largest disk to the to_pole, and after that we
       move the stack from tmp_pole to the to_pole. */

    int tmp_pole = 6 - from_pole - to_pole;

    hanoi_rec_print(num_of_disks - 1, from_pole, tmp_pole);

    hanoi_rec_print(num_of_disks - 1, tmp_pole, to_pole);
    return 1 + ;
  }
}

static void hanoimoves(int num_of_disks)
{
  const int from_pole = 1,
            to_pole = 2;

  hanoi_rec_count(num_of_disks, from_pole, to_pole);
  printf("\n");
}

int main(int argc, char *argv[])
{
  int num_of_disks;

  if (argc != 2 || sscanf(argv[1], "%d", &num_of_disks) != 1 ||
      num_of_disks <= 0)
  {
    fprintf(stderr,"Usage: %s <positive integer>\n", argv[0]);
    return EXIT_FAILURE;
  }

  hanoimoves(num_of_disks);
  return EXIT_SUCCESS;
}
