.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=subsetsum
MAIN=./${FILE_BASE}_mn.x
OBJECTS=${FILE_BASE}.o ${FILE_BASE}_mn.o list_of_numbers.o

ifeq (${WITHDP},yes)
  CFLAGS+=-DWITH_DP_BASED_METHOD
endif

all: ${MAIN}

${MAIN}:${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} -lm

test:test_r_small test_r_large test_m

test_r_small:${MAIN}
	@./${MAIN} -f numbers4.txt -r 15 25 | diff - subsets15-25_4_minsize.txt
	@echo "Congratulations. $@ passed"

test_r_large:${MAIN}
	@./${MAIN} -f numbers40.txt -r 1 1000 | diff - subsets1-1000_40_minsize.txt
	@echo "Congratulations. $@ passed"

test_m:test_m_large test_m_no_sol

test_m_large:${MAIN}
	@./${MAIN} -f numbers40.txt -r 1 1000 -m | diff - subsets1-1000_40_minsize.txt
	@echo "Congratulations. $@ passed"

test_m_no_sol:${MAIN}
	@./${MAIN} -f numbers40.txt -r 10930 10930 -m | diff - no_solution.txt
	@echo "Congratulations. $@ passed"

test_difficult:${MAIN}
	@./${MAIN} -f numbers.txt -r 10930 10930 | diff - no_solution.txt
	@echo "Congratulations. $@ passed"

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

.PHONY:clean
clean:
	${RM} ${MAIN} ${OBJECTS} *.d
	${RM} -r ${MAIN}.dSYM

-include ${wildcard *.d}
