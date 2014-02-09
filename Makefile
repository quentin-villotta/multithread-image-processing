# Makefile for compiling on OS X with Clang++

# Compiler
CC=clang++

# Objects
OBJECTS=*.o

# Compiler flags
CPPFLAGS=""

# Linker flags
#LDFLAGS=-L/opt/local/lib -lSDL -lSDLmain -lboost_program_options-mt -lboost_timer-mt -framework Cocoa

main: main.cpp
	$(CC) $(CPPFLAGS) -c *.cpp
	$(CC) *.o $(CPPFLAGS) $(LDFLAGS)

clean:
	rm *.o a.out
