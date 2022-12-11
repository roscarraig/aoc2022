#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(long*)b - *(long*)a );
}

typedef struct monkey_s {
  unsigned int  items[40], itemcount, par, div, itrue, ifalse, act;
  char op;
} monkey;

void give(monkey *monkeys, int ind, int val)
{
  monkeys[ind].items[monkeys[ind].itemcount++] = val;
}

void run(monkey *monkeys, int count, int worry)
{
  int i;

  for(i = 0; i < count; i++)
  {
    int j;
    for (j = 0; j < monkeys[i].itemcount; j++)
    {
      unsigned long iv = monkeys[i].items[j], target;

      monkeys[i].act++;

      if(monkeys[i].op == '+')
        iv += monkeys[i].par;
      else if(monkeys[i].op == '*')
        iv *= monkeys[i].par;
      else
        iv *= iv;

      if(worry == 1)
        iv /= 3;
      else
        iv %= worry;
      target = (iv % monkeys[i].div == 0) ? monkeys[i].itrue : monkeys[i].ifalse;
      give(monkeys, target, iv);
    }
    monkeys[i].itemcount = 0;
  }
}

void report(monkey *monkeys, int num)
{
  int i, j;
  
  for(i = 0; i < num; i++)
  {
    printf("Monkey %d: ", i);
    for(j = 0; j < monkeys[i].itemcount; j++)
      printf("%d, ", monkeys[i].items[j]);
    printf("\n");
  }
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256];
  monkey monkeys[8], monkeys2[8];
  unsigned int  num = 0, i, worry = 1;
  unsigned long busy[8];

  memset(monkeys, 0, 8 * sizeof(monkey));
  memset(busy, 0, 8 * sizeof(long));

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    char *p;
    int   val;

    fgets(buffer, 256, fp);
    buffer[strlen(buffer) - 1] = 0;
    p = buffer + 18;
    while(p)
    {
      sscanf(p, "%d", &val);
      monkeys[num].items[monkeys[num].itemcount++] = val;
      p = index(p, ',');
      if(p)
        p += 2;
    }
    fgets(buffer, 256, fp);
    monkeys[num].op = buffer[23];
    if(strncmp(buffer + 25, "old", 3) == 0)
      monkeys[num].op = '^';
    else
    {
      sscanf(buffer + 25, "%d\n", &val);
      monkeys[num].par = val;
    }
    fscanf(fp, "  Test: divisible by %d\n", &val);
    monkeys[num].div = val;
    worry *= val;
    fscanf(fp, "    If true: throw to monkey %d\n", &val);
    monkeys[num].itrue = val;
    fscanf(fp, "    If false: throw to monkey %d\n", &val);
    monkeys[num++].ifalse = val;
  }
  memcpy(monkeys2, monkeys, num * sizeof(monkey));

  for(i = 1; i <= 20; i++)
    run(monkeys, num, 1);

  for(i = 0; i < num; i++)
    busy[i] = monkeys[i].act;

  qsort(busy, num, sizeof(int), cmpfunc);
  printf("Part 1: %d\n", busy[0] * busy[1]);

  for(i = 1; i <= 10000; i++)
    run(monkeys2, num, worry);

  for(i = 0; i < num; i++)
    busy[i] = monkeys2[i].act;

  qsort(busy, num, sizeof(long), cmpfunc);
  printf("Part 2: %ld\n", busy[0] * busy[1]);
}
