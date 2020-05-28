.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=pfn_line_store
MAIN=./${FILE_BASE}_mn.x
TEXTFILES=${wildcard *.[chd] *.txt}
OBJECTS=${FILE_BASE}_mn.o ${FILE_BASE}.o pfn_file_info.o

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

.PHONY:test

test:test_l test_r test_o
	@echo "Congratulations. $@ passed"

test_l:${MAIN}
	$(eval TMPFILE := $(shell mktemp))
	@wc -l *.[ox] ${TEXTFILES} | sed -e 's/insgesamt/total/' > ${TMPFILE}
	@${MAIN} -l *.[ox] ${TEXTFILES} | diff -w - ${TMPFILE}
	@${RM} ${TMPFILE}
	@echo "Congratulations. $@ passed"

test_r:${MAIN}
	$(eval TMPFILE := $(shell mktemp))
	@rev ${TEXTFILES} > ${TMPFILE}
	@${MAIN} -r ${TEXTFILES} | diff - ${TMPFILE}
	@${RM} ${TMPFILE}
	@echo "Congratulations. $@ passed"

test_o:${MAIN}
	@${MAIN} -o ${FILE_BASE}_mn.c | ${REVERSE_LINES} | diff - ${FILE_BASE}_mn.c
	@echo "Congratulations. $@ passed"

.PHONY:clean
clean:
	${RM} ${FILE_BASE}_mn.x ${OBJECTS} *.d
	${RM} -r ${FILE_BASE}_mn.x.dSYM

-include ${wildcard *.d}
