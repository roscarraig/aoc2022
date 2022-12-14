#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int xy(int x, int y)
{
  return (x - 300) + y * 200;
}

void scanmax(char *input, int *x1, int *x2, int *y)
{
  char *p = input;

  while(p)
  {
    int i, j;
    
    sscanf(p, "%d,%d", &i, &j);
    if(i < *x1)
      *x1 = i;
    else if(i > *x2)
      *x2 = i;
    if(j > *y)
      *y = j;
    p = index(p, '>');
    if(p)
      p += 2;
  }
}

void drawline(char *map, int x1, int y1, int x2, int y2)
{
  int i, j, k;

  if(x1 == x2)
  {
    if(y1 < y2)
    {
      i = y1;
      j = y2;
    } else {
      i = y2;
      j = y1;
    }
    for(k = i; k <= j; k++)
      map[xy(x1, k)] = '#';
  } else
  {
    if(x1 < x2)
    {
      i = x1;
      j = x2;
    } else {
      i = x2;
      j = x1;
    }
    for(k = i; k <= j; k++)
      map[xy(k, y1)] = '#';
  }
}

void drawmap(char *map, char *input)
{
  char *p = input;
  int   x1, y1, x2, y2;

  while(p)
  {
    sscanf(p, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
    p = index(p, '>');
    if(p)
    {
      drawline(map, x1, y1, x2, y2);
      p += 2;
    }
  }
}

int add_sand(char *map, int x1, int x2, int my)
{
  int x = 500, y = 0;
  
  /* while(x >= x1 && x <= x2 && y <= my) */
  while(y <= my)
  {
    if(map[xy(x, y + 1)] == 0)
      y++;
    else if(map[xy(x - 1, y + 1)] == 0)
    {
      y++;
      x--;
    }
    else if(map[xy(x + 1, y + 1)] == 0)
    {
      y++;
      x++;
    }
    else
    {
      map[xy(x, y)] = '.';
      return 1;
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256], *map = malloc(400 * 200);;
  int   x1 = 500, x2 = 500, y = 0, part1 = 0;

  memset(map, 0, 40000);

  while(!feof(fp) && fgets(buffer, 1024, fp))
  {
    scanmax(buffer, &x1, &x2, &y);
  }
  rewind(fp);
  while(!feof(fp) && fgets(buffer, 1024, fp))
    drawmap(map, buffer);

  while(add_sand(map, x1, x2, y))
    part1++;

  printf("Part 1: %d\n", part1);
  drawline(map, 301, y + 2, 699, y + 4);
  printf("%d\n", y);
  while(add_sand(map, x1, x2, y + 3) && map[xy(500, 0)] == 0)
    part1++;
  printf("Part 2: %d\n", part1 + 1);
  /* 23111 low */
}
