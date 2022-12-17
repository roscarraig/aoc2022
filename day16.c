#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct valve_s {
  int flow;
  int linkcount;
  int links[10];
} valve;

int walk(valve *valves, int count, int pos, int **map, int left, int *taps)
{
  int i, mv = 0, v, *staps;

  if(left < 1)
    return 0;

  staps = malloc(count * sizeof(int));
  memcpy(staps, taps, count * sizeof(int));
  staps[pos] = 1;

  for(i = 0; i < count; i++)
    if(i != pos && staps[i] == 0 && left - 1> map[pos][i])
    {

      v = walk(valves, count, i, map, left - 1 - map[pos][i], staps);

      if(v > mv)
        mv = v;
    }
  free(staps);

  return valves[pos].flow * left + mv;
}

int walk2(valve *valves, int count, int pos1, int pos2, int **map, int left1, int left2, int *taps)
{
  int i, mv = 0, v, *staps;

  if(left1 < 1 || left2 < 1)
    return 0;

  staps = malloc(count * sizeof(int));
  memcpy(staps, taps, count * sizeof(int));
  staps[pos1] = 1;
  staps[pos2] = 1;

  for(i = 0; i < count; i++)
    if(left1 > left2)
    {
      if(i != pos1 && i != pos2 && staps[i] == 0 && left1 - 1> map[pos1][i])
      {
        int l = left1 - 1 - map[pos1][i];
        v = walk2(valves, count, i, pos2, map, l, left2, staps) + valves[i].flow * l;

        if(v > mv)
          mv = v;
      }
    }
    else
    {
      if(i != pos1 && i != pos2 && staps[i] == 0 && left2 - 1> map[pos2][i])
      {
        int l = left2 - 1 - map[pos2][i];
        v = walk2(valves, count, pos1, i, map, left1, l, staps) + valves[i].flow * l;

        if(v > mv)
          mv = v;
      }
    }
  free(staps);

  return mv;
}

void plot_map(int **map, int count, valve *valves)
{
  int res = 1, i, j, a, depth = 0;

  while(res)
  {
    res = 0;
    for(i = 0; i < count; i++)
      for(j = 0; j < count; j++)
        if(map[i][j] == depth)
          for(a = 0; a < valves[j].linkcount; a++) 
            if(map[i][valves[j].links[a]] == -1)
            {
              map[i][valves[j].links[a]] = depth + 1;
              res++;
            }
    depth++;
  }
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256], *labels, tmp[3];
  int   count = 0, i = 0, *flow, **links, *linkcount, *taps, **map;
  valve *valves;

  while(!feof(fp) && fgets(buffer, 256, fp))
    count++;
  rewind(fp);

  labels = malloc(count * 3 + 1);
  memset(labels, 0, count * 3 + 1);
  valves = malloc(count * sizeof(valve));
  memset(valves, 0, count * sizeof(valve));
  taps = malloc(count * sizeof(int));
  memset(taps, 0, count * sizeof(int));

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    sscanf(buffer, "Valve %s has flow rate=%d;", labels + 3 * i, &(valves[i].flow));
    labels[3 * i + 2] = ',';
    if(valves[i].flow == 0)
      taps[i] = 1;
    i++;
  }
  rewind(fp);
  map = malloc(count * sizeof(int *));

  for(i = 0; i < count; i++)
  {
    map[i] = malloc(sizeof(int) * count);
    memset(map[i], -1, count * sizeof(count));
    map[i][i] = 0;
  }
  i = 0;

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    char *p = strstr(buffer, "valve");
    p += 6;
    if( *p == ' ')
      p++;
    while(*p && *p != '\n')
    {
      char check[3], *q;
      int  j;

      check[0] = *p++;
      check[1] = *p++;
      check[2] = 0;
      j = (strstr(labels, check) - labels) / 3;
      valves[i].links[valves[i].linkcount++] = j;
      if(*p == ',')
        p +=2;
    }
    i++;
  }
  plot_map(map, count, valves);
  i = (strstr(labels, "AA") - labels) / 3;
  taps[i] = 1;
  printf("Part 1: %d\n", walk(valves, count, i, map, 30, taps));
  printf("Part 2: %d\n", walk2(valves, count, i, i, map, 26, 26, taps));
}
