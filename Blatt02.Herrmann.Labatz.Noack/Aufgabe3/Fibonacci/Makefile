.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=fibonacci

all:${FILE_BASE}_small.x ${FILE_BASE}_medium.x ${FILE_BASE}_large.x

${FILE_BASE}_small.x:${FILE_BASE}.c
	${CC} -DSMALLVALUES ${CFLAGS} -o $@ $<

${FILE_BASE}_medium.x:${FILE_BASE}.c
	${CC} -DMEDIUMVALUES ${CFLAGS} -o $@ $<

${FILE_BASE}_large.x:${FILE_BASE}.c
	${CC} ${CFLAGS} -o $@ $<

test_small:${FILE_BASE}_small.x 
	@./$< | diff - fibonacci_small.tsv

test_medium:${FILE_BASE}_medium.x 
	@./$< | diff - fibonacci_medium.tsv

test_large:${FILE_BASE}_large.x 
	@./$< | diff - fibonacci_large.tsv

test:test_small test_medium test_large
	@echo "Congratulations. Tests passed"

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

.PHONY:clean
clean:
	@${RM} ${FILE_BASE}.[ox]
	@${RM} -r ${FILE_BASE}_small.x.dSYM ${FILE_BASE}_small.x
	@${RM} -r ${FILE_BASE}_medium.x.dSYM ${FILE_BASE}_medium.x
	@${RM} -r ${FILE_BASE}_large.x.dSYM ${FILE_BASE}_large.x
