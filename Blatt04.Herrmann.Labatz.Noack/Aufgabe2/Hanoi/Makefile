.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=hanoi

all:${FILE_BASE}.x

%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}.x
	${eval TMPFILE := ${shell mktemp}}
	@for disks in 3 4 5 6 7 8; do \
	   ./hanoi.x $${disks} > ${TMPFILE};\
	   ./hanoi_test.rb ${TMPFILE} $${disks};\
	done

.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[ox]
	${RM} -r ${FILE_BASE}.x.dSYM
