#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct valve_s {
  int flow;
  int linkcount;
  int links[10];
} valve;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

int all_open(int *taps, int count)
{
  int i;

  for(i = 0; i < count; i++)
    if(taps[i] == 0)
      return 0;
  return 1;
}

int operate(valve *valves, int count, int *taps, int flow, int left, int pos, int from)
{
  int *results, rescount = valves[pos].linkcount, i;

  if(left == 0)
    return 0;
  if(all_open(taps, count))
    return flow * left;
  if(taps[pos] == 0)
    rescount++;

  results = malloc(sizeof(int) * rescount);
  memset(results, 0, rescount * sizeof(int));

  for(i = 0; i < valves[pos].linkcount; i++)
    if(valves[pos].links[i] != from)
    {
      int *staps = malloc(count * sizeof(int));
      memcpy(staps, taps, count * sizeof(int));
      results[i] = operate(valves, count, staps, 0, left - 1, valves[pos].links[i], pos);
      free(staps);
    }
  if(taps[pos] == 0)
  {
    taps[pos] = 1;
    results[rescount - 1] = operate(valves, count, taps, valves[pos].flow, left - 1, pos, pos);
  }
  qsort(results, rescount, sizeof(int), cmpfunc);
  return(flow * left + results[0]);
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256], *labels, tmp[3];
  int   count = 0, i = 0, *flow, **links, *linkcount, *taps;
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
  printf("Part 1: %d\n", operate(valves, count, taps, 0, 30, 0, 0));
  /* 2113 high */
}
