#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_s {
  char *value;
  struct list_s *next;
  int total;
} list;

typedef struct files_s {
  char *value;
  char *filename;
  struct files_s *next;
  int  size;
  list *path;
} files;

list *add_or_find(list *paths, char *path)
{
  list *this = paths;
  int   pathlen = strlen(path);

  if(this == NULL)
  {
    this = malloc(sizeof(list));
    memset(this, 0, sizeof(list));
    this->value = malloc(pathlen + 1);
    memset(this->value, 0, pathlen + 1);
    strncpy(this->value, path, pathlen);
    return this;
  }
  if(strcmp(this->value, path) == 0)
    return this;
  if(this->next)
    return add_or_find(this->next, path);
  this->next = add_or_find(NULL, path);
  return this->next;
}

files *find_files(files *this, list *path, char *line)
{
  files *cur;
  char *fn = index(line, ' ') + 1;

  for (cur = this; cur; cur = cur->next)
    if(strcmp(fn, this->value) == 0 && this->path == path)
      return cur;
  return NULL;
}

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  char  buffer[256], path[256];
  list *paths = add_or_find(NULL, "root"), *cur = paths;
  files *fns = NULL;
  int    limit1 = 100000, limit2, part1 = 0, part2;

  while(!feof(fp) && fgets(buffer, 256, fp))
  {
    buffer[strlen(buffer) - 1] = 0;
    if(strcmp(buffer, "$ cd /") == 0)
      strcpy(path, "root");
    else if(strcmp(buffer, "$ cd ..") == 0)
      *(rindex(path, '/')) = 0;
    else if(strncmp(buffer, "$ cd ", 5) == 0)
    {
      sprintf(path + strlen(path), "/%s", buffer + 5);
      cur = add_or_find(paths, path);
    }
    else if (strncmp(buffer, "dir ", 4) == 0 || strcmp(buffer, "$ ls") == 0)
    {
    } else if (find_files(fns, cur, buffer) == NULL) {
      files *nfn = malloc(sizeof(files));
      int    len = strlen(index(buffer, ' '));

      nfn->value = malloc(len);
      sscanf(buffer, "%d %s", &(nfn->size), nfn->value);
      nfn->path = cur;
      nfn->next = fns;
      fns = nfn;
    }
  }
  for(cur = paths; cur; cur = cur->next)
  {
    int    len = strlen(cur->value);
    files *item;

    cur->total = 0;

    for(item = fns; item; item = item->next)
      if(strncmp(cur->value, item->path->value, len) == 0)
        cur->total += item->size;

    if(cur->total < limit1)
      part1 += cur->total;
  }
  printf("Part 1: %d\n", part1);
  limit2 = 30000000 - (70000000 - paths->total);
  part2 = paths->total;
  for(cur = paths; cur; cur = cur->next)
    if(cur->total >= limit2 && cur->total < part2)
      part2 = cur->total;
  printf("Part 2: %d\n", part2);
}
