.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=enumM

all:${FILE_BASE}.x

%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:test_Enum test_fit
	@echo "Congratulations. $@ passed"

test_Enum:${FILE_BASE}.x
	@./${FILE_BASE}.x 20 | diff -I '^#' - M20.tsv
	@./${FILE_BASE}.x 100 | diff -I '^#' - M100.tsv
	@./${FILE_BASE}.x 1000 | diff -I '^#' - M1000.tsv
	@echo "Congratulations. $@ passed"

steps.tsv:${FILE_BASE}.x
	@./steps.sh 500 > $@

# You need to provide your own values for a and b, such that runs with success
value_a=120
value_b=9
test_fit:steps.tsv
	@./fit_linear.py -a ${value_a} -b ${value_b} --inputfile steps.tsv 
	@echo "Congratulations. $@ passed"
	
.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[ox]
	${RM} -r ${FILE_BASE}.x.dSYM steps.tsv
