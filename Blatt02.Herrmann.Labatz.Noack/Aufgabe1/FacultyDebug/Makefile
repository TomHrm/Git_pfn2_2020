.PHONY:test
CC?=clang
CFLAGS=-g -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=faculty

all:${FILE_BASE}.x

%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}.x
	@${RM} my_results.tsv && touch my_results.tsv
	@for num in `cut -f 1 results.tsv`; do\
		./${FILE_BASE}.x $$num >> my_results.tsv;\
	done
	@diff my_results.tsv results.tsv 
	@${RM} my_results.tsv
	@echo "Congratulations. Tests passed"

.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[ox]
	${RM} -r ${FILE_BASE}.x.dSYM
