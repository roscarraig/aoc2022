#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  int   p[4], part1 = 0, part2 = 0;;

  while(!feof(fp))
    if (fscanf(fp, "%d-%d,%d-%d\n", &(p[0]), &(p[1]), &(p[2]), &(p[3])) != EOF)
    {
      if ((p[0] <= p[2] && p[1] >= p[3]) || (p[0] >= p[2] && p[1] <= p[3]))
        part1++;
      else if ((p[0] <= p[2] && p[1] >= p[2]) || (p[2] <= p[0] && p[3] >= p[0]))
        part2++;
    }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part1 + part2);
}
