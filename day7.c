#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_marker(char *buffer, int n)
{
  int i;
  char c;

  for(i = 0; i < n; i++)
  {
    c = buffer[i];
    buffer[i] = '_';
    if(index(buffer, c))
    {
      buffer[i] = c;
      return 0;
    }
    buffer[i] = c;
  }
  return 1;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  int   part1 = 0, part2 = 0, p1d = 0;
  char  c, packet[5], message[15];

  memset(packet, 0, 5);
  memset(message, 0, 15);
  while(!feof(fp) && (c = fgetc(fp)))
  {
    if(!p1d)
    {
      packet[part1 % 4] = c;
      part1++;
      if(part1 >= 4 && check_marker(packet, 4))
        p1d = 1;
    }
    message[part2 % 14] = c;
    part2++;
    if(part2 >= 14 && check_marker(message, 14))
      break;
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
