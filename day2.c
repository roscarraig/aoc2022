#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char buffer[256], c1, c2;
  int  scores[9], i, j, part1 = 0, part2 = 0;

  /* Let's precompute the scores of all possible games */

  memset(scores, 0, 9 * sizeof(int));
  for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
      scores[i + j * 3] = j + 1 + (j == i ? 3 : (j == (i + 1) % 3 ? 6 : 0));

  while(!feof(fp))
  {
    fscanf(fp, "%c %c\n", &c1, &c2);
    c1 -= 'A';
    c2 -= 'X';
    part1 += scores[c1 + 3 * c2];
    part2 += 3 * c2 + 1 + (c1 + c2 + 2) % 3;
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}

