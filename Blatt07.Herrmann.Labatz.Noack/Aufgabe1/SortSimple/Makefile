.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=sort_simple
MAIN=./${FILE_BASE}_mn.x
OBJECTS=${FILE_BASE}_mn.o pfn_file_info.o pfn_line_store.o sort_simple.o

SYSTEM?=$(shell uname -s)
ifeq (${SYSTEM},Darwin)
  REVERSE_LINES=tail -r
else
  REVERSE_LINES=tac
endif

all:${FILE_BASE}_mn.x

${FILE_BASE}_mn.x:${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} -lm

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}_mn.x
	${eval TMPFILE := ${shell mktemp}}
	@for filebase in numbers small mixed randnums; do \
	   for opt in "" "-n"; do \
	     ${MAIN} $$opt $${filebase}.txt | diff - $${filebase}_sorted$$opt.txt;\
	     ${MAIN} $$opt -r $${filebase}.txt | ${REVERSE_LINES} | diff - $${filebase}_sorted$$opt.txt;\
	   done \
	done
	@${RM} ${TMPFILE}
	@echo "Congratulations. $@ passed"

.PHONY:test_large
test_large:${FILE_BASE}_mn.x
	${eval TMPFILE := ${shell mktemp}}
	./randnums.py -f 500000 > ${TMPFILE}
	time ${MAIN} -n ${TMPFILE} > /dev/null
	time sort -n ${TMPFILE} > /dev/null
	time ${MAIN} ${TMPFILE} > /dev/null
	time sort ${TMPFILE} > /dev/null
	
.PHONY:clean
clean:
	${RM} ${FILE_BASE}_mn.x ${OBJECTS} *.d
	${RM} -r ${FILE_BASE}_mn.x.dSYM

-include ${wildcard *.d}
