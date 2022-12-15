#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sensor_s {
  int x1, y1, x2, y2, is_at, a, b, dist;
} sensor;

int update_sensor(sensor *this, int at)
{
  this->dist = abs(this->x1 - this->x2) + abs(this->y1 - this->y2);
  if(abs(at - this->y1) <= this->dist)
  {
    this->is_at = 1;
    this->a = this->x1 - abs(this->dist - abs(at - this->y1));
    this->b = this->x1 + abs(this->dist - abs(at - this->y1));
  }
  else
    this->is_at = 0;
  return this->is_at;
}

int is_in(sensor *this, int x)
{
  if(this->is_at && x >= this->a && x <= this->b)
    return 1;
  return 0;
}

long scan(sensor *sensors, int count, int bound)
{
  long x, y, i;

  for(y = 0; y <= bound; y++)
  {
    for(i = 0; i < count; i++)
      update_sensor(&(sensors[i]), y);
    for(x = 0; x <= bound; x++)
    {
      int matched = 0;
      for(i = 0; i < count; i++)
        if(sensors[i].y2 == y && sensors[i].x2 == x)
        {
          matched = 1;
          i = count;
        }
      if(matched)
        continue;
      for(i = 0; i < count; i++)
        if(is_in(&(sensors[i]), x))
        {
          matched = 1;
          x = sensors[i].b;
          i = count;
        }
      if(!matched)
      {
        long m = (x * 4000000 + y);
        return m;
      }
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256];
  int   count = 0, i, j, left = 0, right = 0, part1 = 0, level1, bound2;
  sensor *sensors = NULL;

  sscanf(argv[2], "%d", &level1);
  sscanf(argv[3], "%d", &bound2);
  while(!feof(fp) && fgets(buffer, 256, fp))
    count++;
  rewind(fp);
  sensors = malloc(count * sizeof(sensor));
  memset(sensors, 0, count * sizeof(sensor));
  for(i = 0; i < count; i++)
  {
    int a, b, c, d;
    fscanf(fp, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &a, &b, &c, &d);
    sensors[i].x1 = a;
    sensors[i].y1 = b;
    sensors[i].x2 = c;
    sensors[i].y2 = d;
    if(update_sensor(&(sensors[i]), level1))
    {
      if(left == 0)
        left = sensors[i].a;
      else if(sensors[i].a < left)
        left = sensors[i].a;
      if(right == 0)
        right = sensors[i].b;
      else if(sensors[i].b > right)
        right = sensors[i].b;
    }
  }
  for(j = left; j <= right; j++)
  {
    int bump = 1;
    for(i = 0; i < count; i++)
      if(sensors[i].y2 == level1 && sensors[i].x2 == j)
      {
        bump = 0;
        i = count;
      }
    if(bump)
      for(i = 0; i < count; i++)
        if(is_in(&(sensors[i]), j))
        {
          part1++;
          i = count;
        }
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %ld\n", scan(sensors, count, bound2));
}
