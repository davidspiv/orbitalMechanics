#Per CISP400 makefile tutorial

CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: main.o io.o
	$(CC) main.o io.o -o $(EXENAME)

main.o: src/main.cpp include/io.h
	$(CC) $(CFLAGS) src/main.cpp

io.o: src/io.cpp include/io.h
	$(CC) $(CFLAGS) src/io.cpp

clean:
	rm *.o $(EXENAME)

run:
	./$(EXENAME)
