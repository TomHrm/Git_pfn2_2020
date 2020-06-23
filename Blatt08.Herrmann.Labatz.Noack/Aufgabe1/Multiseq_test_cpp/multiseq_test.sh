#!/bin/sh

set -e -x

program=./multiseq_test.x

${program} -1 sw175.fna | diff - sw175.stat
${program} 60 small.fna | diff - small.fna
${program} 60 sw175.fna | diff - sw175.fna
TMPFILE=`mktemp TMP.XXXXXX` || exit 1
echo ">very long sequence" > ${TMPFILE}
grep -v '^>' sw175.fna | tr -d '\n' >> ${TMPFILE}
echo "" >> ${TMPFILE}
${program} 0 ${TMPFILE} | diff - ${TMPFILE}
rm -f ${TMPFILE}
./check_err.py
./check_err.py --valgrind
