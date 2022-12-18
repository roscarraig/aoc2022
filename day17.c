#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sprite_s {
  int  width;
  int  rows;
  int  top[4];
  int  bottom[4];
  char dot[10];
} sprite;

typedef struct drop_s {
  sprite *shape;
  int pos;
  int height;
} drop;

void load_sprites(sprite *shapes)
{
  shapes[0].width = 4;
  shapes[0].rows = 1;
  memset(shapes[0].top, 0, 4 * sizeof(int));
  memset(shapes[0].bottom, 0, 4 * sizeof(int));
  sprintf(shapes[0].dot, "####");

  shapes[1].width = 3;
  shapes[1].rows = 3;
  shapes[1].bottom[0] = 1;
  shapes[1].bottom[1] = 0;
  shapes[1].bottom[2] = 1;
  shapes[1].top[0] = 1;
  shapes[1].top[1] = 2;
  shapes[1].top[2] = 1;
  sprintf(shapes[1].dot, " # ### # ");

  shapes[2].width = 3;
  shapes[2].rows = 3;
  memset(shapes[2].top, 0, 2 * sizeof(int));
  memset(shapes[2].bottom, 0, 3 * sizeof(int));
  shapes[2].top[2] = 2;
  sprintf(shapes[2].dot, "###  #  #");

  shapes[3].width = 1;
  shapes[3].rows = 4;
  shapes[3].bottom[0] = 0;
  shapes[3].top[0] = 3;
  sprintf(shapes[3].dot, "####");

  shapes[4].width = 2;
  shapes[4].rows = 2;
  shapes[4].top[0] = 1;
  shapes[4].top[1] = 1;
  memset(shapes[4].bottom, 0, 2 * sizeof(int));
  sprintf(shapes[4].dot, "####");
}

int max7(int *floor)
{
  int i, mv = 0;
  for(i = 0; i < 7; i++)
    if(floor[i] > mv)
      mv = floor[i];
  return mv;
}

int overlaps(sprite *shape, char *stack, int left, int alt)
{
  int i, j, res = 0;

  for(i = 0; i < shape->rows; i++)
    for(j = 0; j < shape->width; j++)
      if(stack[(alt + i) * 7 + j + left] == '#' && shape->dot[j + i * shape->width] == '#')
      {
	res = 1;
	i = 5;
	j = 5;
      }
  return res;
}

void print_sprite(sprite *shape, char *stack, int left, int alt, int *floor, char x)
{
  int i, j;

  for(i = 0; i < shape->rows; i++)
    for(j = 0; j < shape->width; j++)
      if(shape->dot[j + i * shape->width] == '#')
      {
        stack[(alt + i) * 7 + j + left] = x;
	if(floor[j + left] < alt + i)
	   floor[j + left] = alt + i;
      }
}

void print_stack(char *stack, int start)
{
  int i, j = 0;
  for(i = 0; i < 7; i++)
    if(stack[start * 7 + i] > ' ')
      j = 1;
  if(j)
  {
    print_stack(stack, start + 1);
    printf("|");
    for(i = 0; i < 7; i++)
      printf("%c", stack[start * 7 + i]);
    printf("|\n");
  }
  else
    printf("-> %d\n", start);
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[10240], stack[4 * 7 * 70000];
  sprite shapes[5];
  int    rocks = 0, moves = 0, floor[7], active = 0, jet = 0, jetlen, i;
  drop   rock;
  long   target2 = 1000000000000, loopstart, loopmod, loopcount, part2a, part2b;

  fgets(buffer, 10240, fp);
  jetlen = strlen(buffer) - 1;
  loopmod = jetlen * 5;
  loopstart = target2 % loopmod + loopmod;
  loopcount = target2 / loopmod;
  // printf("%d %ld %ld\n", jetlen * 5, loopcount, loopstart);
  // printf("%ld %ld\n", loopmod, target2 - (loopcount * loopmod));
  memset(floor, 0, 7 * sizeof(int));
  load_sprites(shapes);
  memset(stack, 32, 4 * 7 * 70000);
  memset(stack, '#', 7);

  for(rocks = 0; rocks < loopstart + loopmod; rocks++)
  {
    int alt = max7(floor) + 4, left = 2, falling = 1;
    int r = rocks % 5, i;

    if(rocks == 2022)
      printf("Part 1: %d\n", max7(floor));
    else if(rocks == loopstart)
      part2a = max7(floor);
    while(falling)
    {
      char c = buffer[jet++];

      jet %= jetlen;

      if(c == '<')
      {
        if(left > 0 && overlaps(&(shapes[r]), stack, left - 1, alt) == 0)
          left--;
      }
      else if(left + shapes[r].width < 7 && overlaps(&(shapes[r]), stack, left + 1, alt) == 0)
        left++;

      if(overlaps(&(shapes[r]), stack, left, alt - 1))
      {
        falling = 0;
	print_sprite(&(shapes[r]), stack, left, alt, floor, '#');
      }
      if(falling)
        alt--;
    }
    for(i = 0; i < shapes[r].width; i++)
      if(alt + shapes[r].top[i] > floor[i + left])
        floor[i + left] = shapes[r].top[i] + alt;
  }
  // part2b = max7(floor);
  // printf("Part 2: %ld\n", (part2b - part2a) * loopcount);
  /* 1575879496604 high */
}
