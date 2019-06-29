GCC=g++
TARGET=server
OBJ_BIN=bin
INCLUDEDIR = src/includes

source = $(wildcard src/*.cpp)
source += $(wildcard src/*/*.cpp)

# OBJFILE = $(addprefix ${OBJ_BIN}/,$(source:%.cpp=%.o))
# OBJFILE = $($(notdir ${source}):%.cpp=%.o)
OBJFILE = $(source:%.cpp=%.o)

all: ${TARGET}

${TARGET}: ${OBJFILE}
	${GCC} $^ -o ${OBJ_BIN}/$@

include $(source:.cpp=.d)

%.d: %.cpp
	set -e; rm -f $@; \
	$(GCC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,\$*.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

test:
	@echo ${OBJFILE}
	@echo $(source)
	@echo $(source:.cpp=.d)

.PHONY: clean
clean:
	rm -f *.o ${DEST} ${OBJFILE} $(source:%.cpp=%.d)
