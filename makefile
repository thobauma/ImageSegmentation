CC=g++
CXXFLAGS=-O3 -std=c++17
CXXFLAGS += -Wall -Wextra -Wpedantic
all: imageSegmentation

bitmap.o: bitmap.cpp bitmap.hpp
	$(CC) $(CXXFLAGS) -c $^

graph.o: graph.cpp graph.hpp
	$(CC) $(CXXFLAGS) -c $^

prGraph.o: prGraph.cpp prGraph.hpp
	$(CC) $(CXXFLAGS) -c $^

main.o: main.cpp graph.hpp prGraph.hpp
	$(CC) $(CXXFLAGS) -c $^

imageSegmentation: main.o graph.o prGraph.o bitmap.o
	$(CC) $(CXXFLAGS) -o $@ $^
clean:
	rm -f *.o 
	rm -f *.hpp.gch
	rm imageSegmentation
	rm images/original/*
	rm images/segmented/*
