CC=g++
CFLAGS=-O3 -std=c++17

all: imageSegmentation

bitmap.o: bitmap.cpp bitmap.h
	$(CC) $(CFLAGS) -c bitmap.cpp

graph.o: graph.cpp graph.h bitmap.h
	$(CC) $(CFLAGS) -c graph.cpp


main.o: main.cpp graph.h bitmap.h
	$(CC) $(CFLAGS) -c main.cpp

imageSegmentation: main.o graph.o bitmap.o
	$(CC) $(CFLAGS) -o imageSegmentation main.o graph.o bitmap.o

clean:
	rm -f *.o 
