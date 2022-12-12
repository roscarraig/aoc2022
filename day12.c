#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int step(char *map, int *route, int w, int h, int num, int end)
{
  int i;
  for(i = 0; i < w * h; i++)
    if(route[i] == num)
    {
      if (i == end)
        return 1;
      if(i > w && route[i - w] == 0 && (map[i - w] - 1) <= map[i])
        route[i - w] = num + 1;
      if(i % w != 0 && route[i - 1] == 0 && (map[i - 1] - 1) <= map[i])
        route[i - 1] = num + 1;
      if(i < w * (h - 1) && route[i + w] == 0 && (map[i + w] - 1) <= map[i])
        route[i + w] = num + 1;
      if(i % w != w - 1 && route[i + 1] == 0 && (map[i + 1] - 1) <= map[i])
        route[i + 1] = num + 1;
    }
  return 0;
}

int step2(char *map, int *route, int w, int h, int num)
{
  int i;
  for(i = 0; i < w * h; i++)
    if(route[i] == num)
    {
      if (map[i] == 'a')
        return 1;
      if(i > w && route[i - w] == 0 && map[i - w] >= map[i] - 1)
        route[i - w] = num + 1;
      if(i % w != 0 && route[i - 1] == 0 && map[i - 1] >= map[i] - 1)
        route[i - 1] = num + 1;
      if(i < w * (h - 1) && route[i + w] == 0 && map[i + w] >= map[i] - 1)
        route[i + w] = num + 1;
      if(i % w != w - 1 && route[i + 1] == 0 && map[i + 1] >= map[i] - 1)
        route[i + 1] = num + 1;
    }
  return 0;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256], *map, *c;
  int   w, h = 1, *route, i = 0, end, done = 0;

  fgets(buffer, 256, fp);
  w = strlen(buffer) - 1;
  while(!feof(fp) && fgets(buffer, 256, fp))
    h++;
  rewind(fp);
  map = malloc(w * h);
  route = malloc(w * h * sizeof(int));
  memset(map, 0, w * h);
  memset(route, 0, w * h * sizeof(int));
  while(!feof(fp) && fgets(buffer, 256, fp))
    memcpy(map + i++ * w, buffer, w);
  c = index(map, 'S');
  *c = 'a';
  route[c - map] = 1;
  c = index(map, 'E');
  *c = 'z';
  end = c - map;
  i = 0;
  while(!done)
    done = step(map, route, w, h, ++i, end);
  printf("Part 1: %d\n", i - 1);
  memset(route, 0, w * h * sizeof(int));
  route[end] = 1;
  i = 0;
  done = 0;
  while(!done)
    done = step2(map, route, w, h, ++i);
  printf("Part 2: %d\n", i - 1);
}
