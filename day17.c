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

void print_sprite(sprite *shape, char *stack, int left, int alt, int *floor)
{
  int i, j;

  for(i = 0; i < shape->rows; i++)
    for(j = 0; j < shape->width; j++)
      if(shape->dot[j + i * shape->width] == '#')
      {
        stack[(alt + i) * 7 + j + left] = '#';
	if(floor[j + left] < alt + i)
	  floor[j + left] = alt + i;
      }
}

void print_stack(char *stack, int start)
{
  int i, j = 0;
  for(i = 0; i < 7; i++)
    if(stack[start * 7 + i] == '#')
      j = 1;
  if(j)
  {
    print_stack(stack, start + 1);
    for(i = 0; i < 7; i++)
      printf("%c", stack[start * 7 + i]);
    printf("\n");
  }
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[10240], stack[4 * 7 * (2022 + 4)];
  sprite shapes[5];
  int    rocks = 0, moves = 0, floor[7], active = 0, jet = 0, jetlen;
  drop   rock;

  fgets(buffer, 10240, fp);
  jetlen = strlen(buffer) - 1;
  memset(floor, 0, 7 * sizeof(int));
  load_sprites(shapes);
  memset(stack, 32, 4 * 7 * 2026);
  memset(stack, '#', 7);

  for(rocks = 0; rocks < 2022; rocks++)
  {
    int alt = max7(floor) + 3, left = 2, falling = 1;
    int r = rocks % 5, i;

    printf("%d %d\n", rocks, alt);
    print_stack(stack, 0);

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
	print_sprite(&(shapes[r]), stack, left, alt, floor);
      }

      /*
      for(i = 0; i < shapes[r].width; i++)
        if(floor[i + left] + 1 == shapes[r].bottom[i] + alt)
          falling = 0;
      */
      if(falling)
        alt--;
    }
    for(i = 0; i < shapes[r].width; i++)
      floor[i + left] = shapes[r].top[i] + alt;
  }
  printf("Part 1: %d\n", max7(floor));
}
