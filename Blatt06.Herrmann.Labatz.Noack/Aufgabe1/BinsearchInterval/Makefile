.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=binsearch_interval
MAIN=${FILE_BASE}_mn.x
OBJECTS=${FILE_BASE}_mn.o ${FILE_BASE}.o

all:${MAIN}

${MAIN}:${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} -lm

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

test:${MAIN}
	@./${MAIN}
	@./${MAIN} -d 1 -i 1
	@./${MAIN} -d 10 -i 1
	@./${MAIN} -d 1 -i 10
	@./${MAIN} -l 10000000
	@echo "Congratulations. $@ passed"

README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

.PHONY:clean
clean:
	${RM} *.[oxd]
	${RM} -r ${MAIN}.x.dSYM
