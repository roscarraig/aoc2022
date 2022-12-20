#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int left, int right, int *numbers)
{
  int t = numbers[left];

  numbers[left] = numbers[right];
  numbers[right] = t;
}

void splice(int ind, int count, int *numbers, int *shadow)
{
  int val = numbers[ind], pos = ind, i, npos;

  val = (val % (count - 1)  + count - 1) % (count - 1);

  if(val > 0)
  {
    for(i = 0; i < val; i++)
    {
      npos = (pos + 1) % count;
      swap(pos, npos, numbers);
      swap(pos, npos, shadow);
      pos = npos;
      if(pos == count - 1 && i < val - 1)
      {
        int j;

        for(j = count - 1; j > 0; j--)
        {
          swap(j, j - 1, numbers);
          swap(j, j - 1, shadow);
        }
        pos = 0;
      }
    }
  }
}

int locate(int ind, int count, int *numbers)
{
  int i;

  for(i = 0; i < count; i++)
    if(numbers[i] == ind)
      return i;
  printf("This should not happen\n");
  return -1;
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  int    count = 0, val, *numbers, *numbers2, *numberso, *shadow, *shadow2, i = 0, j, r, key = 811589153;
  long   part2 = 0;

  while(!feof(fp) && fscanf(fp, "%d\n", &val))
    count++;

  rewind(fp);
  numbers = malloc(count * sizeof(int));
  shadow = malloc(count * sizeof(int));
  numbers2 = malloc(count * sizeof(int));
  shadow2 = malloc(count * sizeof(int));
  numberso = malloc(count * sizeof(int));

  while(!feof(fp) && fscanf(fp, "%d\n", &val))
  {
    int lval = (((val % (count - 1) + count - 1 ) % (count - 1)) * (key % (count - 1))) % (count - 1);
    
    numbers[i] = val;
    numbers2[i] = lval;
    numberso[i] = val;
    shadow[i] = i;
    shadow2[i] = i;
    i++;
  }
  for(i = 0; i < count; i++)
  {
    val = locate(i, count, shadow);
    splice(val, count, numbers, shadow);
  }
  for(i = 0; i < count; i++)
    if(numbers[i] == 0)
    {
      val = i;
      i = count;
    }
  printf("Part 1: %d\n", numbers[(val + 1000) % count] + numbers[(val + 2000) % count] + numbers[(val + 3000) % count]);

  for(r = 0; r < 10; r++)
    for(i = 0; i < count; i++)
    {
      val = locate(i, count, shadow2);
      splice(val, count, numbers2, shadow2);
    }
  for(i = 0; i < count; i++)
    if(numbers2[i] == 0)
    {
      val = i;
      i = count;
    }
  part2 = numberso[shadow2[(val + 1000) % count]];
  part2 += numberso[shadow2[(val + 2000) % count]];
  part2 += numberso[shadow2[(val + 3000) % count]];
  printf("Part 2: %ld\n", part2 * key);
}
