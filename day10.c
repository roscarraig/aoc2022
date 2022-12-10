#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void draw(char *display, int clock, int x)
{
  if(abs(x - clock % 40) < 2)
    display[clock] = '#';
  else
    display[clock] = '.';
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  int   x = 1, cycle = 0, next = 20, part1 = 0;
  char  buffer[32], display[256];

  memset(display, 0, 256);

  while(!feof(fp) && fgets(buffer, 32, fp))
  {
    int bump, val = 0;

    cycle++;
    draw(display, cycle - 1, x);

    if(strcmp(buffer, "noop\n") != 0)
    {
      sscanf(buffer + 5, "%d\n", &val);
      cycle ++;
      draw(display, cycle - 1, x);
    }

    if(cycle >= next)
    {
      part1 += next * x;
      next += 40;
    }
    x += val;
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2:\n");
  for(cycle = 0; cycle < 240; cycle++)
  {
    printf("%c", display[cycle]);
    if(cycle % 40 == 39)
      printf("\n");
  }
}
