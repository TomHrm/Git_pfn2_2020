.PHONY:test
CXX?=clang++
CFLAGS=-g -m64 -Wall -Wunused-parameter -Wunused-variable -O3 -std=c++11
FILE_BASE=multiseq_test
OBJECTS=${FILE_BASE}.o multiseq.o pfn_file_info.o

ifeq ($(assert),no)
  CFLAGS += -DNDEBUG
endif

${FILE_BASE}.x:${OBJECTS}
	${CXX} ${CFLAGS} -o $@ ${OBJECTS} -lm

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

test:${FILE_BASE}.x
	@./multiseq_test.sh
	@echo "Congratulations. $@ passed"

.PHONY:clean
clean:
	${RM} *.[oxd]
	${RM} -r ${FILE_BASE}.x.dSYM __pycache__

# read dependencies
-include $(wildcard *.d)
