#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int priority (char c)
{
  if(c >= 'a' && c <= 'z')
    return(c + 1 - 'a');
  else
    return(c + 27 - 'A');
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char buffer[256], group[2][256];
  int  packsize = 0, i, midpoint, part1 = 0, part2 = 0, line = 0;

  while(!feof(fp))
  {
    memset(buffer, 0, 256);
    fgets(buffer, 256, fp);
    if(!buffer[0])
      continue;
    packsize = strlen(buffer) - 1;
    midpoint = packsize / 2;
    for(i = 0; i < midpoint; i++)
      if(index(buffer + midpoint, buffer[i]))
      {
        part1 += priority(buffer[i]);
        i = midpoint;
      }
    if(line < 2)
      memcpy(group[line], buffer, 256);
    else
      for(i = 0; i < packsize; i++)
        if(index(group[0], buffer[i]) && index(group[1], buffer[i]))
        {
          part2 += priority(buffer[i]);
          i = packsize;
        }
    line = (line + 1) % 3;
  }
  printf("Part1: %d\n", part1);
  printf("Part2: %d\n", part2);
}
