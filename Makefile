GCC=g++
TARGET=server
OBJ_BIN=bin
INCLUDEDIR = src/includes

IncludePath = src/includes

source = $(wildcard src/*.cpp)
source += $(wildcard src/*/*.cpp)

# OBJFILE = $(addprefix ${OBJ_BIN}/,$(source:%.cpp=%.o))
OBJFILE = $(source:%.cpp=%.o)

all: ${TARGET}

${TARGET}: ${OBJFILE}
	${GCC} $^ -o ${OBJ_BIN}/$@

${OBJFILE}:%.o:%.cpp
	@echo compile $@ from $^
	${GCC} $^ -o $@ -I${IncludePath}

include $(source:.cpp=.d)

%.d: %.cpp
	set -e; rm -f $@; \
	$(GCC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

test:
	@echo ${OBJFILE}
	@echo $(source)
	@echo $(source:%.cpp=%.o)

.PHONY: clean
clean:
	rm -f *.o ${DEST}
