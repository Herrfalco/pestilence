#!/usr/bin/env python3

import sys

if __name__ == '__main__':
    val = 0;
    key = 0;
    result = set();
    bits = int(sys.argv[1]);

    for i in range(0, bits):
        key |= val
        key <<= 1
        val ^= 1

    for i in range(0, pow(2, bits)):
        result.add(key & i)
    for pattern in result:
        print(str(pattern) + ": ", end="")
        print([i for i in range(0, pow(2, bits)) if i & key == pattern])
