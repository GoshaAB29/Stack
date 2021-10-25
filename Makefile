CC=g++

CFLAGS=-c -DNDEBUG

all: program

program: main.o stack.o
	$(CC) main.o stack.o -o program

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
stack.o:
	$(CC) $(CFLAGS) C:\Projects\Stack\source\stack.cpp

clean:
	rm -rf *.o program 