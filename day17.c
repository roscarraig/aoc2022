#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sprite_s {
  int  width;
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
  memset(shapes[0].top, 0, 4 * sizeof(int));
  memset(shapes[0].bottom, 0, 4 * sizeof(int));
  sprintf(shapes[0].dot, "####");

  shapes[1].width = 3;
  shapes[1].bottom[0] = 1;
  shapes[1].bottom[1] = 0;
  shapes[1].bottom[2] = 1;
  shapes[1].top[0] = 1;
  shapes[1].top[1] = 2;
  shapes[1].top[2] = 1;

  shapes[2].width = 3;
  memset(shapes[2].top, 0, 2 * sizeof(int));
  memset(shapes[2].bottom, 0, 3 * sizeof(int));
  shapes[2].top[2] = 2;

  shapes[3].width = 1;
  shapes[3].bottom[0] = 0;
  shapes[3].top[0] = 3;

  shapes[4].width = 2;
  shapes[4].top[0] = 1;
  shapes[4].top[1] = 1;
  memset(shapes[4].bottom, 0, 2 * sizeof(int));
}

int max7(int *floor)
{
  int i, mv = 0;
  for(i = 0; i < 7; i++)
    if(floor[i] > mv)
      mv = floor[i];
  return mv;
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[10240], stack[7 * (2022 + 4)];
  sprite shapes[5];
  int    rocks = 0, moves = 0, floor[7], active = 0, jet = 0, jetlen;
  drop   rock;

  fgets(buffer, 10240, fp);
  jetlen = strlen(buffer) - 1;
  memset(floor, 0, 7 * sizeof(int));
  load_sprites(shapes);
  memset(stack, 0, 7 * 2026);

  for(rocks = 0; rocks < 2022; rocks++)
  {
    int alt = max7(floor) + 3, left = 2, falling = 1;
    int r = rocks % 5, i;

    printf("%d %d\n", rocks, alt);

    while(falling)
    {
      char c = buffer[jet++];

      jet %= jetlen;

      if(c == '<')
      {
        if(left > 0)
        {

          left--;
        }
      }
      else if(left + shapes[r].width < 7)
        left++;

      for(i = 0; i < shapes[r].width; i++)
        if(floor[i + left] + 1 == shapes[r].bottom[i] + alt)
          falling = 0;
      if(falling)
        alt--;
    }
    for(i = 0; i < shapes[r].width; i++)
      floor[i + left] = shapes[r].top[i] + alt;
  }
  printf("Part 1: %d\n", max7(floor));
}
