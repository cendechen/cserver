GCC=g++
DEST=server
socure = $(wildcard src/*.cpp src/*/*.cpp)
INCLUDEDIR = src/includes
.PHONY: clean

VPATH=src:./includes

all: ${DEST}

${DEST}: %.o
	${GCC} $^ -o ${DEST}
%.o: ${socure}
	${GCC}  -c ${socure} -I ${INCLUDEDIR}  -o $@

test:
	@echo ${socure}

clean:
	rm -f *.o ${DEST}
