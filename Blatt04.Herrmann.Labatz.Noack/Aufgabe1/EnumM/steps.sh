#!/bin/sh

if test $# -ne 1
then
  echo "Usage: $0 <maximum value of n>"
  exit 1
fi

n=1
max_n=$1
while test ${n} -le ${max_n}
do
  ./enumM.x ${n} | grep '^#' | cut -d '#' -f 2
  n=`expr ${n} + 1`
done
