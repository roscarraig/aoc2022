#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int xy(int x, int y, int mx)
{
  return x + y * mx;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  int   mx, my = 0, m1, m2, i, j, part1 = 0, part2 = 0;
  char  buffer[256], *copse, *seen;

  while(!feof(fp) && fgets(buffer, 256, fp))
    my++;
  mx = strlen(buffer) - 1;
  rewind(fp);
  copse = malloc(mx * my);
  seen = malloc(mx * my);
  memset(seen, 0, mx * my);
  for(j = 0 ; j < my; j++)
  {
    fgets(buffer, 256, fp);
    memcpy(copse + j * mx, buffer, mx);
  }
  for(j = 0; j < my; j++)
  {
    m1 = copse[j * mx];
    m2 = copse[mx + j * mx - 1];
    seen[j * mx] |= 1;
    seen[mx + j * mx - 1] |= 2;
    for (i = 1; i < mx; i++)
    {
      int a, c = 0, s = 1, mh = copse[xy(i, j, mx)];

      if(mh > m1)
      {
        m1 = mh;
        seen[i + j * mx] |= 1;
      }
      if(copse[mx + j * mx - 1 - i] > m2)
      {
        m2 = copse[mx + j * mx - 1 - i];
        seen[mx + j * mx - 1 - i] |= 2;
      }
      for(a = i - 1; a >= 0; a--)
      {
        c++;
        if(copse[xy(a, j, mx)] >= mh)
          a = -1;
      }
      s *= c;
      c = 0;
      for(a = i + 1; a < mx; a++)
      {
        c++;
        if(copse[xy(a, j, mx)] >= mh)
          a = mx;
      }
      s *= c;
      c = 0;
      for(a = j - 1; a >= 0; a--)
      {
        c++;
        if(copse[xy(i, a, mx)] >= mh)
          a = -1;
      }
      s *= c;
      c = 0;
      for(a = j + 1; a < my; a++)
      {
        c++;
        if(copse[xy(i, a, mx)] >= mh)
          a = my;
      }
      s *= c;
      if(s > part2)
        part2 = s;
    }
  }
  for(i = 0; i < mx; i++)
  {
    m1 = copse[i];
    m2 = copse[my * mx + i - mx];
    seen[i] |= 4;
    seen[my * mx + i - mx] |= 8;
    for (j = 1; j < my; j++)
    {
      int j0 = mx * (my - 1 - j);

      if(copse[i + j * mx] > m1)
      {
        m1 = copse[i + j * mx];
        seen[i + j * mx] |= 4;
      }
      if(copse[i + j0] > m2)
      {
        m2 = copse[i + j0];
        seen[i + j0] |= 8;
      }
    }
  }
  for(i = 0; i < mx * my; i++)
    if(seen[i])
      part1++;
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
