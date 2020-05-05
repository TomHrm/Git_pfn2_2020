.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=ascii

all:${FILE_BASE}.x

%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}.x
	@./${FILE_BASE}.x | diff - ascii_table.tsv
	@echo "Congratulations. Tests passed"

.PHONY:clean
clean:
	@${RM} ${FILE_BASE}.[ox]
	@${RM} -r ${FILE_BASE}.x.dSYM
