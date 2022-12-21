#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct monkey_s {
  char name[8], a[8], b[8], op;
  int  hit, ai, bi;
  long val;
} monkey;

int mindex(char *list, char *want)
{
  return((int) (strstr(list, want) - list) / 5);
}

long mval(monkey *monkeys, char *list, int ind)
{
  long av, bv;

  if(monkeys[ind].hit == 1)
    return monkeys[ind].val;

  av = mval(monkeys, list, mindex(list, monkeys[ind].a));
  bv = mval(monkeys, list, mindex(list, monkeys[ind].b));

  switch(monkeys[ind].op)
  {
    case '+':
      return av + bv;
    case '-':
      return av - bv;
    case '*':
      return av * bv;
    case '/':
      return av / bv;
  }
}

int has(monkey *monkeys, char *list, int ind, int seek)
{
  int ai, bi;

  if(ind == seek)
    return 1;
  if(monkeys[ind].hit)
    return 0;

  ai = mindex(list, monkeys[ind].a);
  bi = mindex(list, monkeys[ind].b);

  return has(monkeys, list, ai, seek) + has(monkeys, list, bi, seek);
}

long resolve(monkey *monkeys, char *list, int ind, int hind, long want)
{
  long need;
  int  lr, ai, bi, ha, hb;

  if(ind == hind)
    return want;


  ai = mindex(list, monkeys[ind].a);
  bi = mindex(list, monkeys[ind].b);
  ha = has(monkeys, list, ai, hind);
  hb = has(monkeys, list, bi, hind);

  if(ha == hb)
    printf("This should not happen\n");

  if(ha)
  {
    lr = 0;
    need = mval(monkeys, list, bi);
  }
  else
  {
    lr = 1;
    need = mval(monkeys, list, ai);
  }

  switch(monkeys[ind].op)
  {
    case '=':
      if(lr == 0)
        return resolve(monkeys, list, ai, hind, need);
      else
        return resolve(monkeys, list, bi, hind, need);
    case '+':
      if(lr == 0)
        return resolve(monkeys, list, ai, hind, want - need);
      else
        return resolve(monkeys, list, bi, hind, want - need);
    case '-':
      if(lr == 0)
        return resolve(monkeys, list, ai, hind, want + need);
      else
        return resolve(monkeys, list, bi, hind, need - want);
    case '*':
      if(lr == 0)
        return resolve(monkeys, list, ai, hind, want / need);
      else
        return resolve(monkeys, list, bi, hind, want / need);
    case '/':
      if(lr == 0)
        return resolve(monkeys, list, ai, hind, want * need);
      else
        return resolve(monkeys, list, bi, hind, need / want);
  }
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[256], name[8], a[8], b[8], op, *mlist;
  int    count = 0, i = 0, rootind, humind;
  long   val;
  monkey *monkeys;

  while(!feof(fp) && fgets(buffer, 25, fp))
    count++;

  monkeys = malloc(sizeof(monkey) * count);
  memset(monkeys, 0, sizeof(monkey) *count);
  mlist = malloc(count * 5 + 1);
  memset(mlist, 0, count * 5 + 1);
  rewind(fp);

  while(!feof(fp) && fgets(buffer, 25, fp))
  {
    int n = sscanf(buffer, "%s %s %c %s\n", name, a, &op, b);

    strncpy(mlist + i * 5, name, 4);
    mlist[i * 5 + 4] = ':';

    if(n == 4)
    {
      strncpy(monkeys[i].name, name, 4);
      strncpy(monkeys[i].a, a, 4);
      strncpy(monkeys[i].b, b, 4);
      monkeys[i].op = op;
    } else {
      sscanf(buffer, "%s %ld\n", name, &val);
      strncpy(monkeys[i].name, name, 4);
      monkeys[i].val = val;
      monkeys[i].hit = 1;
    }
    i++;
  }
  for(i = 0; i < count; i++)
  {
    if(monkeys[i].hit == 0)
    {
      monkeys[i].ai = mindex(mlist, monkeys[i].a);
      monkeys[i].bi = mindex(mlist, monkeys[i].b);
    }
  }
  rootind = mindex(mlist, "root");
  humind = mindex(mlist, "humn");

  printf("Part 1: %ld\n", mval(monkeys, mlist, rootind));
  monkeys[rootind].op = '=';
  printf("Part 2: %ld\n", resolve(monkeys, mlist, rootind, humind, 0));
}
