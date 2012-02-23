#make file for cv

CFLAGS=`pkg-config --cflags opencv` `pkg-config --libs opencv` -Wall
CC=g++
EXECUTEABLE=roboboat

OBJECTS = stdafx.o main.o pathPlanning.o imageProc.o locomotion.o rs232.o

all: roboboat.o

roboboat.o: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTEABLE)

main.o: Roboboat.cpp
	$(CC) $(CFLAGS) -c Roboboat.cpp -o main.o

rs232.o: rs232.c
	$(CC) $(CFLAGS) -c rs232.c -o rs232.o

locomotion.o: locomotion.cpp
	$(CC) $(CFLAGs) -c locomotion.cpp

pathPlanning.o: pathPlanning.cpp
	$(CC) $(CFLAGS) -c pathPlanning.cpp

imageProc.o: imageProc.cpp
	$(CC) $(CFLAGS) -c imageProc.cpp

stdafx.o: stdafx.cpp
	$(CC) $(CFLAGS) -c stdafx.cpp

clean:
	rm -rf *o $(EXECUTEABLE)
