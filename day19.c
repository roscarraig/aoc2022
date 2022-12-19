#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct blueprint_s {
  int id, ore_o, clay_o, obs_o, obs_c, geo_o, geo_b;
} blueprint;

int run(blueprint *bp, int turn, int maxturn, int *rcount, int *rescount)
{
  int best = 0;
  int v, rob[4], res[4], can[4], i;

  if(turn == maxturn)
    return rescount[3];

  if(bp->geo_b <= rescount[2] && bp->geo_o <= rescount[0])
  {
    memcpy(rob, rcount, sizeof(rob));
    memcpy(res, rescount, sizeof(res));

    for(i = 0; i < 4; i++)
      res[i] += rob[i];

    rob[3]++;
    res[2] -= bp->geo_b;
    res[0] -= bp->geo_o;
    v = run(bp, turn + 1, maxturn, rob, res);
    if(v > best)
      best = v;
  }
  if(bp->obs_c <= rescount[1] && bp->obs_o <= rescount[0] && rcount[2] < bp->geo_b)
  {
    memcpy(rob, rcount, sizeof(rob));
    memcpy(res, rescount, sizeof(res));

    for(i = 0; i < 4; i++)
      res[i] += rob[i];

    rob[2]++;
    res[1] -= bp->obs_c;
    res[0] -= bp->obs_o;
    v = run(bp, turn + 1, maxturn, rob, res);
    if(v > best)
      best = v;
  }
  if(bp->clay_o <= rescount[0]  && (rescount[0] - rcount[0]) <= bp->clay_o  && (rcount[1] < bp->obs_c))
  {
    memcpy(rob, rcount, sizeof(rob));
    memcpy(res, rescount, sizeof(res));

    for(i = 0; i < 4; i++)
      res[i] += rob[i];

    rob[1]++;
    res[0] -= bp->clay_o;
    v = run(bp, turn + 1, maxturn, rob, res);
    if(v > best)
      best = v;
  }
  if(bp->ore_o <= rescount[0] && ((rescount[0] - rcount[0] < bp->ore_o) || (rescount[0] - rcount[0]) < bp->clay_o))
  {
    memcpy(rob, rcount, sizeof(rob));
    memcpy(res, rescount, sizeof(res));

    for(i = 0; i < 4; i++)
      res[i] += rob[i];

    rob[0]++;
    res[0] -= bp->ore_o;
    v = run(bp, turn + 1, maxturn, rob, res);
    if(v > best)
      best = v;
  }

  memcpy(rob, rcount, sizeof(rob));
  memcpy(res, rescount, sizeof(res));

  for(i = 0; i < 4; i++)
    res[i] += rob[i];

  v = run(bp, turn + 1, maxturn, rob, res);

  if(v > best)
    best = v;

  return best;
}

int main(int argc, char **argv)
{
  FILE  *fp = fopen(argv[1], "r");
  char   buffer[256];
  int    count = 0, i = 0, part1 = 0, part2 = 1;
  blueprint *bps;
  int rob[] = {1, 0, 0, 0}, res[] = {0, 0, 0, 0}, v;

  while(!feof(fp) && fgets(buffer, 256, fp))
    count++;
  rewind(fp);

  bps = malloc(sizeof(blueprint) * count);
  memset(bps, 0, sizeof(blueprint) * count);

  while(!feof(fp) && fscanf(fp,
        "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.\n",
        &(bps[i].id), &(bps[i].ore_o), &(bps[i].clay_o), &(bps[i].obs_o), &(bps[i].obs_c), &(bps[i].geo_o), &(bps[i].geo_b)))
  {
    int rob[] = {1, 0, 0, 0}, res[] = {0, 0, 0, 0}, v;
    v = run(&(bps[i]), 0, 24, rob, res);
    i++;
    part1 += v * i;
  }
  printf("Part 1: %d\n", part1);
  
  for(i = 0; i < 3; i++)
  {

    v = run(&(bps[i]), 0, 32, rob, res);
    part2 *= v;
  }
  printf("Part 2: %d\n", part2);
}
