#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[256], digits[] = "012=-";
  long   total = 0, i, j, num;

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    j = 1;
    num = 0;

    for(i = strlen(buffer) - 2; i >= 0; i--)
    {
      switch(buffer[i])
      {
        case '-':
          num += -1 * j;
          break;

        case '=':
          num += -2 * j;
          break;

        default:
          num += (buffer[i] - '0') * j;
          break;
      }
      j *= 5;
    }
    total += num;
  }
  i = 0;
  j = 0;;

  while(total > 0 || j)
  {
    int k = total % 5;
    total -= k;
    total /= 5;
    k += j;
    j = 0;

    if(k > 2)
      j = 1;
    if(k == 5)
      k = 0;

    buffer[i++] = digits[k];
  }
  i--;

  printf("Part 1: ");
  for(; i >= 0; i--)
    printf("%c", buffer[i]);
  printf("\n");
}
