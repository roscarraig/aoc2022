#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int box[25][25][25];

typedef struct cube_s {
  int side[6];
  int axis[3];
} cube;

int adjacent(cube *cubes, int a, int b)
{
  int i, score = 0, side;

  for(i = 0; i < 3; i++)
  {
    if(cubes[a].axis[i] == cubes[b].axis[i])
      score++;
    else if(abs(cubes[a].axis[i] - cubes[b].axis[i]) > 1)
      return 0;
    else
      side = (1 + i) * (cubes[a].axis[i] - cubes[b].axis[i]);
  }
  return score == 2 ? side : 0;
}

void add_cube(cube *cubes, int ind, int x, int y, int z)
{
  int i, side;

  cubes[ind].axis[0] = x;
  cubes[ind].axis[1] = y;
  cubes[ind].axis[2] = z;

  for(i = 0; i < 6; i++)
    cubes[ind].side[i] = -1;

  for(i = 0; i < ind; i++)
  {
    side = adjacent(cubes, i, ind);
    if(side)
    {
      if(side < 0)
      {
        cubes[i].side[2 - side] = ind;
        cubes[ind].side[-1 - side] = i;
      } else {
        cubes[i].side[side - 1] = ind;
        cubes[ind].side[side + 2] = i;
      }
    }
  }
}

void find_bounds(cube *cubes, int count, int *bounds)
{
  int i;

  for(i = 0; i < count; i++)
  {
    int j;
    for(j = 0; j < 3; j++)
    {
      if(bounds[j] == -1 || cubes[i].axis[j] < bounds[j])
        bounds[j] = cubes[i].axis[j];
      if(bounds[j + 3] == -1 || cubes[i].axis[j] > bounds[j + 3])
        bounds[j + 3] = cubes[i].axis[j];
    }
  }
}

int wave(int x, int y, int z, int *bounds, cube *cubes)
{
  int i, axes[] = {x, y, z};

  for(i = 0; i < 3; i++)
    if(axes[i] < 0 || axes[i] > bounds[i + 3] + 2)
      return 0;

  if(box[axes[0]][axes[1]][axes[2]] != 0)
    return 0;

  box[axes[0]][axes[1]][axes[2]] = -1;

  for(i = 0; i < 3; i++)
  {
    int n;
    if(axes[i] > 0)
    {
      n = box[i == 0 ? x - 1 : x][i == 1 ? y - 1 : y][i == 2 ? z - 1 : z];

      if(n > 0)
        cubes[n - 1].side[i + 3] = - 2;
      else if (n == 0)
        wave(i == 0 ? x - 1 : x, i == 1 ? y - 1 : y, i == 2 ? z - 1 : z, bounds, cubes);
    }
    n = box[i == 0 ? x + 1 : x][i == 1 ? y + 1 : y][i == 2 ? z + 1 : z];

    if(n > 0)
      cubes[n - 1].side[i] = - 2;
    else if (n == 0)
      wave(i == 0 ? x + 1 : x, i == 1 ? y + 1 : y, i == 2 ? z + 1 : z, bounds, cubes);
  }
  return 0;
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  int    count = 0, ind = 0, part1 = 0, part2 = 0, x, y, z, i, j, bounds[6];
  char   buffer[256];
  cube  *cubes;

  while(!feof(fp) && fgets(buffer, 256, fp))
    count++;
  rewind(fp);
  cubes = malloc(sizeof(cube) * count);
  memset(box, 0, sizeof(box));

  for(i = 0; i < 6; i++)
    bounds[i] = -1;

  while(!feof(fp) && fscanf(fp, "%d,%d,%d\n", &x, &y, &z))
  {
    add_cube(cubes, ind++, x, y, z);
    box[x][y][z] = ind;
  }

  for(ind = 0; ind < count; ind++)
    for(i = 0; i < 6; i++)
      if(cubes[ind].side[i] == -1)
        part1++;
  printf("Part 1: %d\n", part1);
  find_bounds(cubes, count, bounds);
  
  for(i = 0; i < count; i++)
    for(j = 0; j < 3; j++)
    {
      if(cubes[i].axis[j] == bounds[j] && cubes[i].side[j] == -1)
        cubes[i].side[j] = -2;
      if(cubes[i].axis[j] == bounds[j + 3] && cubes[i].side[j + 3] == -1)
        cubes[i].side[j + 3] = -2;
    }
  wave(0, 0, 0, bounds, cubes);
  // wave(bounds[0] + 1, bounds[1] + 1, bounds[2] + 1, bounds, cubes);
  for(ind = 0; ind < count; ind++)
  {
    for(i = 0; i < 6; i++)
      if(cubes[ind].side[i] == -2)
        part2++;
  }
  printf("Part 2: %d\n", part2);
}
