#!/usr/bin/env python3

import json
import sys

from functools import cmp_to_key


def packet_compare(a, b):
    for i in range(min(len(a), len(b))):
        if type(a[i]) == int and type(b[i]) == int:
            if a[i] < b[i]:
                return 1
            if a[i] > b[i]:
                return -1
            continue
        elif type(a[i]) == int:
            v = packet_compare([a[i]], b[i])
        elif type(b[i]) == int:
            v = packet_compare(a[i], [b[i]])
        else:
            v = packet_compare(a[i], b[i])
        if v!=0:
            return v
    if len(a) < len(b):
        return 1
    if len(b) < len(a):
        return -1
    return 0


def __main__():
    part1 = 0
    packets = [[[2]], [[6]]]
    i = 0
    with open(sys.argv[1], 'r') as fhan:
        line1 = fhan.readline().strip()
        while(line1):
            i += 1
            a = json.loads(line1)
            b = json.loads(fhan.readline().strip())
            v = packet_compare(a, b)
            if v > 0:
                part1 += i
            line1 = fhan.readline().strip()
            line1 = fhan.readline().strip()
            packets.append(a)
            packets.append(b)
    print("Part 1:", part1)
    output = sorted(packets, key=cmp_to_key(packet_compare))[::-1]
    print("Part 2:", (output.index([[2]]) + 1) * (output.index([[6]]) + 1))


if __name__ == '__main__':
    __main__()
