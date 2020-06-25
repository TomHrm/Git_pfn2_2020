#!/usr/bin/env python3

import argparse, shutil, os
from mysubprocess import mysubprocess_expect

def parse_arguments():
  p = argparse.ArgumentParser(description='run program for error cases')
  p.add_argument('-v','--valgrind',action='store_true',default=False,
                  help='run program with valgrind')
  return p.parse_args()

program='./multiseq_test.x'

args = parse_arguments()

if args.valgrind and os.uname().sysname != 'Darwin' and \
                     shutil.which('valgrind'):
  valgrind_opts = ['--quiet', '--tool=memcheck', '--memcheck:leak-check=full',
                   '--memcheck:leak-resolution=high','--error-exitcode=1',
                   '--log-fd=1','--error-limit=yes','--dsymutil=yes']
  prefix = 'valgrind {} '.format(' '.join(valgrind_opts))
else:
  prefix = ''

'''
each call to the following dictionary entries represent a corrupted
file and the expected error message with placeholders for the
prefix and the filename, when applying the multiseq_test.x to that file.
the expected error code (which is EXIT_FAILURE in the
C/C++-source) is always 1.
'''

files2msg \
  = {'empty.fna'          : ' is empty',
     'only_header.fna'    : ': sequence number 1 is empty',
     'only_sequence.fna'  : ': missing header for sequence 1',
     'first_corrupt.fna'  : ': sequence number 1 is empty',
     'second_corrupt.fna' : ': sequence number 2 is empty'
    }

expected_error_code = 1
for filename, msg in files2msg.items():
  full_msg = '{}: file "{}"{}'.format(program,filename,msg)
  mysubprocess_expect('{}{} 60 {}'.format(prefix,program,filename),
                      expected_error_code,
                      full_msg)
