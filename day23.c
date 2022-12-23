#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct elf_s {
  int x, y, newx, newy, blocked;
} elf;

int occupied(elf *mob, int count, int x, int y)
{
  int i;

  for(i = 0; i < count; i++)
    if(mob[i].x == x && mob[i].y == y)
      return i;
  return -1;
}

int scan(elf *mob, int count, int ind, int dir)
{
  int dx, dy, dd, i, j, border[3][3], lone = 1;

  mob[ind].newx = mob[ind].x;
  mob[ind].newy = mob[ind].y;

  for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
      if(i != 1 || j != 1)
      {
        border[i][j] = occupied(mob, count, mob[ind].x + i - 1, mob[ind].y + j - 1);
        if(border[i][j] != -1)
          lone = 0;
      }

  if(lone)
  {
    mob[ind].blocked = 1;
    return 0;
  }

  mob[ind].blocked = 0;

  for(dd = 0; dd < 4; dd++)
  {
    switch((dir + dd) % 4)
    {
      case 0:
        dx = 0;
        dy = 1;
        break;

      case 1:
        dx = 0;
        dy = -1;
        break;

      case 2:
        dx = -1;
        dy = 0;
        break;

      case 3:
        dx = 1;
        dy = 0;
        break;
    }
    if(border[1 + dx][1 + dy] != -1)
      continue;
    if((dir + dd) % 4 < 2 && (border[0][1 + dy] != -1 || border[2][1 + dy] != -1))
      continue;
    if((dir + dd) % 4 > 1 && (border[1 + dx][0] != -1 || border[1 + dx][2] != -1))
      continue;
    mob[ind].newx = mob[ind].x + dx;
    mob[ind].newy = mob[ind].y + dy;

    for(i = 0; i < ind; i++)
      if(mob[i].newx == mob[ind].newx && mob[i].newy == mob[ind].newy)
      {
        mob[i].blocked = 1;
        mob[ind].blocked = 1;
        return 0;
      }
    return 1;
  }
  mob[ind].blocked = 1;
  return 0;
}

void find_bounds(elf *mob, int count, int *bounds)
{
  int i;

  bounds[0] = mob[0].x;
  bounds[1] = mob[0].y;
  bounds[2] = mob[0].x;
  bounds[3] = mob[0].y;

  for(i = 1; i < count; i++)
  {
    if(mob[i].x < bounds[0])
      bounds[0] = mob[i].x;
    if(mob[i].x > bounds[2])
      bounds[2] = mob[i].x;
    if(mob[i].y < bounds[1])
      bounds[1] = mob[i].y;
    if(mob[i].y > bounds[3])
      bounds[3] = mob[i].y;
  }
}

void print_map(elf *mob, int count)
{
  int bounds[4], i, j;

  find_bounds(mob, count, bounds);

  for(j = bounds[3]; j >= bounds[1]; j--)
  {
    for(i = bounds[0]; i <= bounds[2]; i++)
    {
      int v = occupied(mob, count, i, j);
      printf("%c", v == -1 ? '.' : '#');
    }
    printf("\n");
  }
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[256];
  int    count = 0, dir = 0, i = 0, n = 0, bounds[4], moves, lines = 0;
  elf   *mob;

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    char *p = buffer;

    while(*p)
    {
      if(*p == '#')
        count++;
      p++;
    }
    lines++;
  }
  i = lines - 1;
  mob = malloc(count * sizeof(elf));
  rewind(fp);

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    int j;

    for(j = 0; j < strlen(buffer); j++)
    {
      if(buffer[j] == '#')
      {
        mob[n].x = j;
        mob[n].y = i;
        n++;
      }
    }
    i--;
  }
  for(i = 0; i < 10; i++)
  {
    moves = 0;

    for(n = 0; n < count; n++)
      moves += scan(mob, count, n, i);
    for(n = 0; n < count; n++)
      if(!mob[n].blocked)
      {
        mob[n].x = mob[n].newx;
        mob[n].y = mob[n].newy;
      }
  }
  find_bounds(mob, count, bounds);
  printf("Part 1: %d %d\n", (bounds[2] + 1 - bounds[0]) * (bounds[3] + 1 - bounds[1]) - count, i);
  while(moves > 0)
  {
    moves = 0;
    i++;
    for(n = 0; n < count; n++)
      scan(mob, count, n, i - 1);
    for(n = 0; n < count; n++)
      if(!mob[n].blocked)
      {
        mob[n].x = mob[n].newx;
        mob[n].y = mob[n].newy;
        moves++;
      }
  }
  printf("Part 2: %d\n", i);
}
