#!/usr/bin/env python3

import sys


def move(ends, d, knots):
    if (d == 'L'):
        ends[0] -= 1
    elif (d == 'R'):
        ends[0] += 1
    elif (d == 'U'):
        ends[1] += 1
    else:
        ends[1] -= 1

    for i in range(1, knots):
        if ends[(i - 1) * 2] - ends[i * 2] == 2:
            ends[i * 2] += 1
            if ends[1 + i * 2] < ends[1 + (i - 1) * 2]:
                ends[1 + i * 2] += 1
            elif ends[1 + i * 2] > ends[1 + (i - 1) * 2]:
                ends[1 + i * 2] -= 1
        elif ends[i * 2] - ends[(i - 1) * 2] == 2:
            ends[i * 2] -= 1
            if ends[1 + i * 2] < ends[1 + (i - 1) * 2]:
                ends[1 + i * 2] += 1
            elif ends[1 + i * 2] > ends[1 + (i - 1) * 2]:
                ends[1 + i * 2] -= 1
        elif ends[1 + (i - 1) * 2] - ends[1 + i * 2] == 2:
            ends[1 + i * 2] += 1
            if ends[i * 2] < ends[(i - 1) * 2]:
                ends[i * 2] += 1
            elif ends[i * 2] > ends[(i - 1) * 2]:
                ends[i * 2] -= 1
        elif ends[1 + i * 2] - ends[1 + (i - 1) * 2] == 2:
            ends[1 + i * 2] -= 1
            if ends[i * 2] < ends[(i - 1) * 2]:
                ends[i * 2] += 1
            elif ends[i * 2] > ends[(i - 1) * 2]:
                ends[i * 2] -= 1
        else:
            break


def __main__():
    ends = [0] * 20
    part1 = set()
    part2 = set()
    with open(sys.argv[1], 'r') as fhan:
        for line in fhan.readlines():
            parts = line.strip().split(' ')
            for i in range(int(parts[1])):
                move(ends, parts[0], 10)
                add = ":{}-{};".format(ends[2], ends[3])
                part1.add(add)
                add = ":{}-{};".format(ends[18], ends[19])
                part2.add(add)
    print("Part 1:", len(part1))
    print("Part 2:", len(part2))


if __name__ == '__main__':
    __main__()
