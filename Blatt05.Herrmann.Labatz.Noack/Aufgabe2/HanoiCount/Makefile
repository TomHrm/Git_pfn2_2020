.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=hanoi_count

all:${FILE_BASE}.x

%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}.x
	@./${FILE_BASE}.x 20 | diff - hanoi_count1-20.txt
	@echo "Congratulations. Test passed"

.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[ox]
	${RM} -r ${FILE_BASE}.x.dSYM
