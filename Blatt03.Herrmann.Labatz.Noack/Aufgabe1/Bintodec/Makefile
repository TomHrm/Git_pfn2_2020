.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=bintodec

OBJECTS=${FILE_BASE}_mn.o ${FILE_BASE}.o

all:${FILE_BASE}_mn.x

${FILE_BASE}_mn.x:${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} -lm

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}_mn.x
	@./${FILE_BASE}_mn.x
	@./${FILE_BASE}_mn.x '00000111 11111110 10001111 11010001'
	@./bintodec_test.sh bitvectors.txt bitvectors_invalid.txt | diff - bitvectors.txt
	@echo "Congratulations. Tests passed"

.PHONY:clean
clean:
	${RM} ${FILE_BASE}_mn.x ${OBJECTS} *.d
	${RM} -r ${FILE_BASE}_mn.x.dSYM

-include ${wildcard *.d}
