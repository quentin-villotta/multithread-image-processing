# Makefile for compiling on OS X with Clang++

# Compiler
CC=clang++

# Objects
OBJECTS=*.o

# Linker flags
LDFLAGS=''

debug: main.cpp
	$(CC) -c *.cpp -g -std=c++11
	$(CC) *.o $(LDFLAGS) -g -std=c++11

release: main.cpp
	$(CC) -c *.cpp -O3 -std=c++11
	$(CC) *.o $(LDFLAGS) -O3 -std=c++11

clean:
	rm *.o a.out
