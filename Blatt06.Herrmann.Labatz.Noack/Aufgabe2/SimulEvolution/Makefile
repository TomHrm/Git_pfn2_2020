.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=simul_evolution
MAIN=./${FILE_BASE}_mn.x
OBJECTS=${FILE_BASE}_mn.o ${FILE_BASE}.o

${FILE_BASE}_mn.x:${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} -lm

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

test:${FILE_BASE}_mn.x
	@./run_simul.sh
	@echo "Congratulations. $@ passed"

.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[oxd] ${FILE_BASE}_mn.[oxd] results.test9.tsv
	${RM} -r ${FILE_BASE}.x.dSYM
