#include <stdio.h>
#include <string.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void store(int *cals, int value)
{
  if(value > cals[0])
  {
    cals[0] = value;
    qsort(cals, 3, sizeof(int), cmpfunc);
  }
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char buffer[256];
  int total = 0, cal;
  int cals[3];

  memset(cals, 0, 3 * sizeof(int));

  while(!feof(fp))
  {
    memset(buffer, 0, 256);
    fgets(buffer, 256, fp);
    if(strlen(buffer) < 2)
    {
      store(cals, total);
      total = 0;
    }
    else
    {
      sscanf(buffer, "%d", &cal);
      total += cal;
    }
  }
  store(cals, total);
  printf("Part 1: %d\n", cals[2]);
  printf("Part 2: %d\n", cals[2] + cals[1] + cals[0]);
}
