#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct map_row_s {
  char *line;
  int   left, right;
} map_row;

typedef struct map_st {
  map_row *rows;
  int      row_count;
  int      max_width;
  int     *top;
  int     *bottom;
} maps;

void print_map(maps *map)
{
  int i, j;

  for(i = 0; i < map->row_count; i++)
  {
    for(j = 0; j <= map->rows[i].right; j++)
      printf("%c", map->rows[i].line[j]);
    printf("\n");
  }
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[6000];
  maps   map;
  int    i = 0, j, x = 0, y = 0, dir = 0, dist = 0, x2 = 0, y2 = 0, dir2 = 0;
  char  *p;

  map.row_count = 0;
  map.max_width = 0;

  while(!feof(fp) && fgets(buffer, 6000, fp) && strlen(buffer) > 2)
    map.row_count++;

  rewind(fp);
  map.rows = malloc(sizeof(map_row) * map.row_count);

  while(!feof(fp) && fgets(buffer, 6000, fp) && strlen(buffer) > 2)
  {
    int j = 0;

    map.rows[i].right = strlen(buffer) - 2;
    map.rows[i].left = 0;
    map.rows[i].line = malloc(map.rows[i].right + 2);
    strncpy(map.rows[i].line, buffer, map.rows[i].right + 1);
    map.rows[i].line[map.rows[i].right + 2] = 0;
    
    if(map.rows[i].right > map.max_width)
      map.max_width = map.rows[i].right;

    while(map.rows[i].line[map.rows[i].left] == ' ')
      map.rows[i].left++;

    i++;
  }
  map.top = malloc(map.max_width * sizeof(int));
  map.bottom = malloc(map.max_width * sizeof(int));
  memset(map.bottom, 0, map.max_width * sizeof(int));

  for(i = 0; i < map.max_width; i++)
    map.top[i] = -1;

  for(i = 0; i < map.row_count; i++)
    for(j = map.rows[i].left; j <= map.rows[i].right; j++)
    {
      if(map.top[j] == -1)
        map.top[j] = i;
      map.bottom[j] = i;
    }
  fgets(buffer, 6000, fp);
  buffer[strlen(buffer) - 1] = 'S';
  p = buffer;

  x = map.rows[0].left;
  x2 = map.rows[0].left;
  while(1)
  {
    if(*p >= '0' && *p <= '9')
    {
      dist = 10 * dist + *p - '0';
    } else {
      for(i = 0; i < dist; i++)
      {
        switch(dir)
        {
          case 0:
            if(x < map.rows[y].right)
            {
              if(map.rows[y].line[x + 1] != '#')
                x++;
            } else
              if(map.rows[y].line[map.rows[y].left] != '#')
                x = map.rows[y].left;
            break;

          case 1:
            if(y < map.bottom[x])
            {
              if(map.rows[y + 1].line[x] != '#')
                y++;
            } else
              if(map.rows[map.top[x]].line[x] != '#')
                y = map.top[x];
            break;

          case 2:
            if(x > map.rows[y].left)
            {
              if(map.rows[y].line[x - 1] != '#')
                x--;
            } else {
              if(map.rows[y].line[map.rows[y].right] != '#')
                x = map.rows[y].right;
            }
            break;

          case 3:
            if(y > map.top[x])
            {
              if(map.rows[y - 1].line[x] != '#')
                y--;
            } else
              if(map.rows[map.bottom[x]].line[x] != '#')
                y = map.bottom[x];
            break;
        }
        switch(dir2) {
          case 0:
            if(x2 < map.rows[y2].right)
            {
              if(map.rows[y2].line[x2 + 1] != '#')
                x2++;
            } else {
              int tx, ty, tdir;

              if(y2 < 50)
              {
                tx = 99; ty = 149 - y2; tdir = 2;
              } else if (y2 < 100) {
                tx = 50 + y2; ty = 49; tdir = 3;
              } else if (y2 < 150) {
                tx = 149; ty = 149 - y2; tdir = 2;
              } else {
                tx = y2 - 100; ty = 149; tdir = 3;
              }
              if(map.rows[ty].line[tx] != '#')
              {
                x2 = tx;
                y2 = ty;
                dir2 = tdir;
              }
            }
            break;

          case 1:
            if(y2 < map.bottom[x2])
            {
              if(map.rows[y2 + 1].line[x2] != '#')
                y2++;
            } else {
              int tx, ty, tdir;

              if(x2 < 50)
              {
                tx = 100 + x2; ty = 0; tdir = 1;
              } else if (x2 < 100) {
                tx = 49; ty = x2 + 100; tdir = 2;
              } else {
                tx = 99; ty = x2 - 50; tdir = 2;
              }
              if(map.rows[ty].line[tx] != '#')
              {
                x2 = tx;
                y2 = ty;
                dir2 = tdir;
              }
            }
            break;

          case 2:
            if(x2 > map.rows[y2].left)
            {
              if(map.rows[y2].line[x2 - 1] != '#')
                x2--;
            } else {
              int tx, ty, tdir;

              if(y2 < 50)
              {
                tx = 0; ty = 149 - y2; tdir = 0;
              } else if (y2 < 100) {
                tx = y2 - 50; ty = 100; tdir = 1;
              } else if (y2 < 150) {
                tx = 50; ty = 149 - y2; tdir = 0;
              } else {
                tx = y2 - 100; ty = 0; tdir = 1;
              }
              if(map.rows[ty].line[tx] != '#')
              {
                x2 = tx;
                y2 = ty;
                dir2 = tdir;
              }
            }
            break;

          case 3:
            if(y2 > map.top[x2])
            {
              if(map.rows[y2 - 1].line[x2] != '#')
                y2--;
            } else {
              int tx, ty, tdir;

              if(x2 < 50)
              {
                tx = 50; ty = x2 + 50; tdir = 0;
              } else if(x2 < 100) {
                tx = 0; ty = x2 + 100; tdir = 0;
              } else {
                tx = x2 - 100; ty = 199; tdir = 3;
              }
              if(map.rows[ty].line[tx] != '#')
              {
                x2 = tx;
                y2 = ty;
                dir2 = tdir;
              }
            }
            break;
        }
      }
      dist = 0;

      if(*p == 'S')
        break;
      if(*p == 'L')
      {
        dir = (3 + dir) % 4;
        dir2 = (3 + dir2) % 4;
      }
      else if(*p == 'R')
      {
        dir = (1 + dir) % 4;
        dir2 = (1 + dir2) % 4;
      }
    }
    p++;
  }
  printf("Part 1: %d\n", 1000 * (y + 1) + 4 * (x + 1) + dir);
  printf("Part 2: %d\n", 1000 * (y2 + 1) + 4 * (x2 + 1) + dir2);
}
