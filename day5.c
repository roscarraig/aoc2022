#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack_s {
  char *pile;
  int   cursor;
} stack;

int box_count(char *row)
{
  int total = 0;
  while((row = index(row, ']')))
  {
    total++;
    row++;
  }
  return total;
}

stack *new_stacks(int count, int height)
{
  stack *result = malloc(sizeof(stack) * count);
  int    i;

  for(i = 0; i < count; i++)
  {
    result[i].pile = malloc(height);
    memset(result[i].pile, 0, height);
    result[i].cursor = -1;
  }
  return result;
}

void moven(stack *stacks, int n, int from, int to)
{
  strncpy(stacks[to].pile + stacks[to].cursor + 1, stacks[from].pile + stacks[from].cursor - n + 1, n);
  stacks[to].cursor += n;
  stacks[from].cursor -= n;
}

void print_top(stack *stacks, int count)
{
  int i;
  for(i = 0; i < count; i++)
    printf("%c", stacks[i].pile[stacks[i].cursor]);
  printf("\n");
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char buffer[256];
  stack *stacks1 = NULL, *stacks2 = NULL;
  int    stacknum = 0, boxnum = 0, rowcount = 1, i;

  fgets(buffer, 256, fp);
  stacknum = strlen(buffer) / 4;
  boxnum += box_count(buffer);
  while(!feof(fp) && !stacks1)
  {
    fgets(buffer, 256, fp);
    if (buffer[1] != '1')
    {
      rowcount++;
      boxnum += box_count(buffer);
    }
    else
    {
      stacks1 = new_stacks(stacknum, boxnum);
      stacks2 = new_stacks(stacknum, boxnum);
    }
  }
  rewind(fp);
  for(i = 0; i < rowcount; i++)
  {
    int j;

    fgets(buffer, 256, fp);

    for(j = 0; j < stacknum; j++)
      if(buffer[j * 4 + 1] != ' ')
      {
        stacks1[j].pile[rowcount - i - 1] = buffer[j * 4 + 1];
        stacks2[j].pile[rowcount - i - 1] = buffer[j * 4 + 1];
        if(stacks1[j].cursor == -1)
        {
          stacks1[j].cursor = rowcount - i - 1;
          stacks2[j].cursor = rowcount - i - 1;
        }
      }
  }
  fgets(buffer, 256, fp);
  fgets(buffer, 256, fp);
  while(!feof(fp))
  {
    int num, from, to;
    if(fscanf(fp, "move %d from %d to %d\n", &num, &from, &to))
    {
      int i;

      for(i = 0; i < num; i++)
        moven(stacks1, 1, from - 1, to - 1);
      moven(stacks2, num, from - 1, to - 1);
    }
  }
  printf("Part 1: ");
  print_top(stacks1, stacknum);
  printf("Part 2: ");
  print_top(stacks2, stacknum);
}
