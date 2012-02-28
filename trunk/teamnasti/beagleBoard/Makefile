#make file for cv

CFLAGS=`pkg-config --cflags opencv` `pkg-config --libs opencv` -Wall
CC=g++
EXECUTEABLE=roboboat

OBJECTS = stdafx.o main.o pathPlanning.o imageProc.o locomotion.o rs232.o blob.o blobR.o\
	CompLabel.o blobP.o blobO.o blobC.o

all: roboboat.o

roboboat.o: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTEABLE)

main.o: Roboboat.cpp
	$(CC) $(CFLAGS) -c Roboboat.cpp -o main.o

blob.o: blob/blob.cpp
	$(CC) $(CFLAGS) -c blob/blob.cpp -o blob.o
	
blobR.o: blob/BlobResult.cpp
	$(CC) $(CFLAGS) -c blob/BlobResult.cpp -o blobR.o
	
CompLabel.o: blob/ComponentLabeling.cpp
	$(CC) $(CFLAGS) -c blob/ComponentLabeling.cpp -o CompLabel.o

blobP.o: blob/BlobProperties.cpp
	$(CC) $(CFLAGS) -c blob/BlobProperties.cpp -o blobP.o

blobO.o: blob/BlobOperators.cpp
	$(CC) $(CFLAGS) -c blob/BlobOperators.cpp -o blobO.o

blobC.o: blob/BlobContour.cpp
	$(CC) $(CFLAGS) -c blob/BlobContour.cpp -o blobC.o		

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
