#!/usr/bin/env python3

import sys


def __main__():
    cp = None
    files = {}
    paths = {}
    tree = {}
    files = {}
    limit1 = 100000
    with open(sys.argv[1]) as fhan:
        line = fhan.readline().strip()
        while(line):
            if line == '$ cd /':
                cp = 'root'
            elif line == '$ cd ..':
                cp = cp[:cp.rindex('/')]
            elif line.startswith('$ cd '):
                cp += '/' + line[5:]
            elif line.startswith('dir ') or line == '$ ls':
                paths[cp] = 0
            else:
                parts = line.split(' ')
                files[cp + ':' + parts[1]] = int(parts[0])
            line = fhan.readline().strip()

    for x in paths:
        tree[x] = 0
        for y in files:
            if y.startswith(x + '/') or y.startswith(x + ':'):
                tree[x] += files[y]

    print("Part 1: {}".format(sum([tree[x] for x in tree if tree[x] < limit1])))
    limit2 = 30000000 - (70000000 - tree['root'])
    print("Part 2: {}".format(sorted([tree[x] for x in tree if tree[x] >= limit2])[0]))


if __name__ == '__main__':
    __main__()
