GCC=g++
DEST=main

main: main.o
	${GCC} main.o -o ${DEST}
main.o: src/main.cpp
	${GCC}  -c src/main.cpp -o main.o

clean:
	rm -f *.o main
