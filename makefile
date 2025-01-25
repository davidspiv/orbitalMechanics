#Per CISP400 makefile tutorial

CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/io.o build/util.o
	$(CC) build/main.o build/io.o build/util.o -o $(EXENAME)

build/main.o: src/main.cpp include/io.h include/util.h
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/io.o: src/io.cpp include/io.h
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build/util.o: src/util.cpp include/util.h
	$(CC) $(CFLAGS) src/util.cpp -o build/util.o

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
