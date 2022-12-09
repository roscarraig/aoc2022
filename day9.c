#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void move(int *ends, char dir, int knots)
{
  int i;
  if(dir == 'L')
    ends[0]--;
  else if(dir == 'R')
    ends[0]++;
  else if(dir == 'U')
    ends[1]++;
  else
    ends[1]--;

  for(i = 1; i < knots; i++)
  {
    if(ends[(i - 1) * 2] - ends[i * 2] == 2)
    {
      ends[i * 2]++;
      if(ends[1 + i * 2] < ends[1 + (i - 1) * 2])
        ends[1 + i * 2]++;
      else if(ends[1 + i * 2] > ends[1 + (i - 1) * 2])
        ends[1 + i * 2]--;
    } else if (ends[i * 2] - ends[(i - 1) * 2] == 2) {
      ends[i * 2]--;
      if(ends[1 + i * 2] < ends[1 + (i - 1) * 2])
        ends[1 + i * 2]++;
      else if(ends[1 + i * 2] > ends[1 + (i - 1) * 2])
        ends[1 + i * 2]--;
    } else if (ends[1 + (i - 1) * 2] - ends[1 + i * 2] == 2) {
      ends[1 + i * 2]++;
      if(ends[i * 2] < ends[(i - 1) * 2])
        ends[i * 2]++;
      else if(ends[i * 2] > ends[(i - 1) * 2])
        ends[i * 2]--;
    } else if (ends[1 + i * 2] - ends[1 + (i - 1) * 2] == 2) {
      ends[1 + i * 2]--;
      if(ends[i * 2] < ends[(i - 1) * 2])
        ends[i * 2]++;
      else if(ends[i * 2] > ends[(i - 1) * 2])
        ends[i * 2]--;
    } else
      i = knots;
  }
}

char *growmap(char *map, int *mapsize)
{
  char *result = realloc(map, *mapsize + 1024);
  memset(result + *mapsize, 0, 1024);
  *mapsize += 1024;
  return result;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char dir;
  int  ends[20], dist, i, mapsize1 = 1024, mapsize2 = 1024, part1 = 0, part2 = 0;
  char *map1 = malloc(1024), *map2 = malloc(1024), buffer[256];

  memset(ends, 0, 20 * sizeof(int));
  memset(map1, 0, mapsize1);
  memset(map2, 0, mapsize2);

  while(!feof(fp) && fscanf(fp, "%c %d\n", &dir, &dist))
  {
    if(strlen(map1) + 256 > mapsize1)
      map1 = growmap(map1, &mapsize1);
    if(strlen(map2) + 256 > mapsize2)
      map2 = growmap(map2, &mapsize2);

    for(i = 0; i < dist; i++)
    {
      move(ends, dir, 10);
      sprintf(buffer,":%d-%d;", ends[2], ends[3]);
      if(!strstr(map1, buffer))
      {
        sprintf(&(map1[strlen(map1)]), "%s", buffer);
        part1++;
      }
      sprintf(buffer,":%d-%d;", ends[18], ends[19]);
      if(!strstr(map2, buffer))
      {
        sprintf(&(map2[strlen(map2)]), "%s", buffer);
        part2++;
      }
    }
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
