#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void move(int ind, int pos, int *numbers)
{
  int val = numbers[ind], i;

  if(pos > ind)
  {
    for(i = ind; i < pos; i++)
      numbers[i] = numbers[i + 1];
    numbers[pos] = val;
  } else {
    for(i = ind; i > pos; i--)
      numbers[i] = numbers[i - 1];
    numbers[pos] = val;
  }
}

void swap(int left, int right, int *numbers)
{
  int t = numbers[left];

  numbers[left] = numbers[right];
  numbers[right] = t;
}

void splice(int ind, int count, int *numbers, int *shadow)
{
  int val = numbers[ind], pos = ind, i, npos;

  while(val < -count)
    val += count - 1;

  if(val >= count)
    val %= count - 1;

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
  else if(val < 0)
  {
    for(i = 0; i < -val; i++)
    {
      npos = (count + pos - 1) % count;
      swap(pos, npos, numbers);
      swap(pos, npos, shadow);
      pos = npos;

      if(pos == 0 && i < -val - 1)
      {
        int j;

        for(j = 0; j < count - 1; j++)
        {
          swap(j, j + 1, numbers);
          swap(j, j + 1, shadow);
        }
        pos = count - 1;
      }
    }
  }
}


void splice0(int ind, int count, int *numbers, int *shadow)
{
  int val = numbers[ind], dir, pos;

  if(val == 0)
    return;

  while(val < 0)
    val += count;
  if(val >= count)
    val %= count;

  pos = (ind + val + count - 1) % count;

  while(pos < 0)
    pos += count;

  if(val < 0 && pos == 0)
    pos = count - 1;
  
  move(ind, pos, numbers);
  move(ind, pos, shadow);
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
  int    count = 0, val, *numbers, *shadow, i = 0;

  while(!feof(fp) && fscanf(fp, "%d\n", &val))
    count++;

  rewind(fp);
  numbers = malloc(count * sizeof(int));
  shadow = malloc(count * sizeof(int));

  while(!feof(fp) && fscanf(fp, "%d\n", &val))
  {
    numbers[i] = val;
    shadow[i] = i;
    i++;
  }
  for(i = 0; i < count; i++)
  {
    int j;

    val = locate(i, count, shadow);
    splice(val, count, numbers, shadow);

    /*
    for(j = 0; j < count; j++)
      printf("%d ", numbers[j]);
    printf("\n");
    for(j = 0; j < count; j++)
      printf("%d ", shadow[j]);
    printf("\n");
    */
  }
  for(i = 0; i < count; i++)
    if(numbers[i] == 0)
    {
      val = i;
      i = count;
    }
  printf("Part 1: %d\n", numbers[(val + 1000) % count] + numbers[(val + 2000) % count] + numbers[(val + 3000) % count]);
}
