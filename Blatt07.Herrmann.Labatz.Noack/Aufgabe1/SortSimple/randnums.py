#!/usr/bin/env python3

import argparse, random

def parse_arguments():
  p = argparse.ArgumentParser(description='generate random numbers on stdout')
  p.add_argument('-f','--float',action='store_true',default=False,
                  help='generate floating point numbers')
  p.add_argument('-c','--columns',type=int, choices=range(1, 10),default=1,
                  help='specify the number of values per line to output')
  p.add_argument('number',type=int,
                 help='specify number of values/lines to generate')
  return p.parse_args()

args = parse_arguments()

def rgenerator(outfloat):
  maxrand = 1000000
  offset = maxrand/2
  while True:
    if outfloat:
      r = random.uniform(float(-offset),float(maxrand - offset))
      yield ("{:.8f}".format(r))
    else:
      yield str(random.randint(-offset,maxrand - offset))

row = list()
for idx, r in enumerate(rgenerator(args.float)):
  if idx >= args.columns * args.number:
    break
  row.append(r)
  if len(row) == args.columns:
    print('{}'.format('\t'.join(row)))
    row.clear()
