#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct snow_s {
  int dir;
  int x;
  int y;
} snow;

void print_map(int *map, char *map2, int mx, int my)
{
  int x, y;
  
  for(y = 0; y < my; y++)
  {
    for(x = 0; x < mx; x++)
      printf("%02d%c", map[x + y * mx], map2[x + y * mx] ? '@' + map2[x + y * mx] : ' ');
    printf("\n");
  }
  printf("\n");
}

void prep_map(snow *storm, char *map, int bcount, int mx, int my)
{
  int i;

  memset(map, 0, mx * my);
  for(i = 0; i < bcount; i++)
  {
    switch(storm[i].dir)
    {
      case 0:
        storm[i].x = (storm[i].x + 1) % mx;
        break;

      case 1:
        storm[i].y++;

        if(storm[i].y == my - 1)
          storm[i].y = 1;
        break;

      case 2:
        storm[i].x--;

        if(storm[i].x == -1)
          storm[i].x = mx - 1;
        break;

      case 3:
        storm[i].y--;
        
        if(storm[i].y == 0)
          storm[i].y = my - 2;
        break;
    }
    map[storm[i].x + (storm[i].y) * mx] |= (1 << storm[i].dir);
  }
}

int next(int *map, char *map2, int move, int x, int y, int mx, int my, int phase)
{
  int c = 0;

  if(y > 1 && map[x + (y - 1) * mx] < move && map2[x + (y - 1) * mx] == 0)
  {
    map[x + (y - 1) * mx] = move + 1;
    c++;
  }
  if(y < my - 2 && map[x + (y + 1) * mx] < move && map2[x + (y + 1) * mx] == 0)
  {
    map[x + (y + 1) * mx] = move + 1;
    c++;
  }
  if(y == my - 2 && x == mx - 1 && phase == 0)
  {
    map[mx * my - 1] = move + 1;
    c++;
  }
  if(y == 1 && x == 0 && phase == 1)
  {
    map[0] = move + 1;
    c++;
  }
  if(x > 0 && y > 0 && y < my - 1 && map[x + y * mx - 1] < move && map2[x + y * mx - 1] == 0)
  {
    map[x + y * mx - 1] = move + 1;
    c++;
  }
  if(x < mx - 1 && y > 0 && y < my - 1 && map[x + y * mx + 1] < move && map2[x + y * mx + 1] == 0)
  {
    map[x + y * mx + 1] = move + 1;
    c++;
  }
  return c;
}

void step(int *map, char *map2, int move, int mx, int my, int phase)
{
  int x, y;

  for(y = 0; y < my; y++)
    for(x = 0; x < mx; x++)
      if(map[x + y * mx] == move)
      {
        if(map2[x + y * mx])
        {
          map[x + y * mx] = 0;
        }
        next(map, map2, move, x, y, mx, my, phase);
        map[x + y * mx]++;
      }
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[256], *map2;
  int    bcount, mx, my = 1, *map, i = 0, x, y = 0, move = 1;
  snow  *storm;

  fgets(buffer, 256, fp);
  mx = strlen(buffer) - 3;

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    char *p = buffer + 1;

    my++;
    while(*p != '#')
    {
      if(*p != '.')
        bcount++;
      p++;
    }
  }
  rewind(fp);
  fgets(buffer, 256, fp);
  storm = malloc(sizeof(snow) * bcount);
  map = malloc(sizeof(int) * mx * my);
  map2 = malloc(mx * my);
  memset(map, 0, sizeof(int) * mx * my);
  memset(map2, 0, mx * my);

  for(y = 1; y < my - 1; y++)
  {
    char *p = buffer + 1;

    fgets(buffer, 256, fp);
    x = 0;

    while(*p != '#')
    {
      if(*p != '.')
      {
        storm[i].x = x;
        storm[i].y = y;

        switch(*p)
        {
          case '>':
            storm[i].dir = 0;
            break;

          case 'v':
            storm[i].dir = 1;
            break;

          case '<':
            storm[i].dir = 2;
            break;

          case '^':
            storm[i].dir = 3;
            break;

          default:
            printf("This should not happen: '%c'\n", *p);
            break;
        }
        i++;
      }
      p++;
      x++;
    }
  }
  map[0] = 1;

  while(map[mx * my - 1] == 0)
  {
    prep_map(storm, map2, bcount, mx, my);
    step(map, map2, move, mx, my, 0);
    move++;
  }
  printf("Part 1: %d\n", move - 1);
  memset(map, 0, sizeof(int) * (mx * my - 1));
  while(map[0] == 0)
  {
    prep_map(storm, map2, bcount, mx, my);
    step(map, map2, move, mx, my, 1);
    move++;
  }
  memset(map + 1, 0, sizeof(int) * (mx * my - 1));
  while(map[mx * my - 1] == 0)
  {
    prep_map(storm, map2, bcount, mx, my);
    step(map, map2, move, mx, my, 0);
    move++;
  }
  printf("Part 2: %d\n", move - 1);
}
