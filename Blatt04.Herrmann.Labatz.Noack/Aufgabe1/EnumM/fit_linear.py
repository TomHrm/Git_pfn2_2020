#!/usr/bin/env python3

import argparse, sys, math
import numpy as np

def fit_linear(n_vec,steps_vec):
  def func(x, a, b):
    return a + b * x
  (a, b) , _ = opt.curve_fit(func, n_vec, steps_vec)
  return a, b

def parse_arguments():
  p = argparse.ArgumentParser(description='fit data to linear function')
  p.add_argument('-a','--avalue',metavar='<float>',type=float,default=None,
                 help='specify value of a')
  p.add_argument('-b','--bvalue',type=float,metavar='<float>',default=None,
                 help='specify value of b')
  p.add_argument('-i','--inputfile',type=str,default='steps.tsv',
                  help='specify input file')
  return p.parse_args()

args = parse_arguments()

''' read the data from the tab separated file '''

steps_for_n = np.loadtxt(args.inputfile,dtype=int)

'''
transform matrix (i.e. array of rows, each of length 2) into
matrix of two 2 rows, one with the x-value, and the other with the
y-value. This is just a transposition, expressed by .T
'''

n_vec, steps_vec = steps_for_n.T

'''
if user gives values for a and b, then take these. Otherwise,
use values which are determined by the curve_fit function of SciPy.
The latter should give some good estimate of a and b which however
does not always provide an upper bound. One should therefore
try values slightly larger than until all steps-values are upperbounded.
'''
fitted = False
if args.avalue and args.bvalue:
  a = args.avalue
  b = args.bvalue
else:
  import scipy.optimize as opt
  a, b = fit_linear(n_vec,steps_vec)
  fitted = True

sum_difference = 0
upper_bound = True
for n, steps in zip(n_vec,steps_vec):
  steps_upper = int(math.ceil(a + n * b))
  if steps > steps_upper:
    sys.stderr.write(('no upper bound: steps = {} > {} = {:.2f} + {:.2f} * {} '
                      '= a + b * n\n').format(steps,steps_upper,a,b,n))
    upper_bound = False
  else:
    sum_difference += (steps_upper - steps)
if upper_bound:
  print('''# values for a and b such that s(n) <= a + b * n for all pairs
# n where s(n) is the number of steps required by your program to compute 
# set M for given parameter n.
# So a and b are the constants such that the linear function f(n)=a+b*n
# is an upper bound for the number of steps in the range of values considered.
# Last value is sum of positive differences f(n) - s(n) for all n.''')
  print('{}\t{}\t{}'.format(a,b,sum_difference))
else:
  if fitted:
    sys.stderr.write('fitted:\ta={:.2f}\tb={:.2f}\n'.format(a,b))
  exit(1)
