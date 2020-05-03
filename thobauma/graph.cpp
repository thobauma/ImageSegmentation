#include <iostream>
#include <queue>
#include <fstream>
#include <string>

#include "graph.h"

Graph::Graph(unsigned int numNodes)
    : numNodes_{numNodes} 
{
    nodes_.resize(numNodes_);
}

Graph::~Graph()
{
    nodes_.clear();
}

void Graph::addEdge(indType start, indType end, unsigned int capacity)
{
    if(start < numNodes_)
        nodes_[start].neighbors_[end] = capacity;
    else
        std::cerr << "Node index out of bound!" << std::endl;
}

