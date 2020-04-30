#!/usr/bin/env python3

import random

print('#/bin/sh')
print('set -e -x')
for trial in range(1000):
  r1 = random.randint(0,1000000)
  r2 = random.randint(0,1000000)
  print('./sumup.x {} {}'.format(r1,r1+r2))
