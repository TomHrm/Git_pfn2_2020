.PHONY:test
CXX?=clang++
LD=${CXX}
CFLAGS=-g -O3 -Wall -Werror -Wunused-parameter -Wpointer-arith --std=c++11
LDFLAGS=-g -lm
OBJEVALHASHFUNC=hashcoll.o tokenizer.o hashfunctions.o

all:hashcoll.x

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

hashcoll.x:${OBJEVALHASHFUNC}
	${LD} ${LDFLAGS} -o $@ ${OBJEVALHASHFUNC}

.PHONY:test
test:test_small test_large
	@echo "Contratulations. $@ passed"

.PHONY:test_large
test_large:hashcoll.x
	@./hashcoll.x shaks.data | diff - shaks.data.tsv
	@echo "Contratulations. $@ passed"

.PHONY:test_small
test_small:hashcoll.x
	@./hashcoll.x collisions.txt | diff - collisions.txt.tsv
	@echo "Contratulations. $@ passed"

.PHONY:clean
clean:
	${RM} *.[oxd]

-include $(wildcard *.d)
