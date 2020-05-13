#!/bin/sh

if test $# -ne 2
then
  echo "Usage: $0 <file with valid bitvectors> <file with invalid bitvectors>"
  exit 1
fi

valid_inputfile=$1
invalid_inputfile=$2
for bitvector in `cut -d '=' -f 2 $valid_inputfile | sed -e 's/ //g'`
do
  ./bintodec_mn.x $bitvector
  if test $? -ne 0
  then
    echo "$0: FAILURE OF ./bintodec_mn.x $bitvector"
    exit 1
  fi
done
for bitvector in `cat $invalid_inputfile`
do
  this_bitvector=`echo ${bitvector} | sed -e 's/_/ /g'`
  ./bintodec_mn.x "$this_bitvector"
  if test $? -eq 0
  then
    echo "$0: illegal input: EXPECT FAILURE of ./bintodec_mn.x $this_bitvector"
    exit 1
  fi
done
