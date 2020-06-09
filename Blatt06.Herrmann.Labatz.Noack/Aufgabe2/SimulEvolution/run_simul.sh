#!/bin/sh

set -e -x

PROGNAME=./simul_evolution_mn.x

TMPFILE=`mktemp TMP.XXXXXX` || exit 1
${PROGNAME} -m 1000 -p 0.0 -n 1000 -q 1.0 | tee ${TMPFILE}
grep -q fixed:1 ${TMPFILE}
if test $? -ne 0
then
  echo "$0: unexpected result for simulation 1"
  exit 1
fi
rm -f ${TMPFILE}

${PROGNAME} -m 1000 -p 1.0 -n 1000 -q 0.0 | tee ${TMPFILE}
grep -q fixed:0 ${TMPFILE}
if test $? -ne 0
then
  echo "$0: unexpected result for simulation 2"
  exit 1
fi
rm -f ${TMPFILE}

${PROGNAME} -m 1000 -p 0.0 -n 1000 -q 0.0 | tee ${TMPFILE}
grep -q "simulation stopped" ${TMPFILE}
if test $? -ne 0
then
  echo "$0: unexpected result for simulation 3"
  exit 1
fi
rm -f ${TMPFILE}

${PROGNAME} -m 1000 -p 0.5 -n 1000 -q 0.5
${PROGNAME} -m 1000 -p 0.6 -n 1000 -q 0.5
${PROGNAME} -m 1000 -p 1.0 -n 1000 -q 1.0
${PROGNAME} -m 2000 -p 0.51 -n 100000 -q 0.5
${PROGNAME} -m 2000 -p 0.5 -n 100000 -q 0.51
${PROGNAME} -m 2000 -p 0.5 -n 100000 -q 0.51 -l results.test9.tsv
