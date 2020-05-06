#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <limits>

#include "graph.h"

#define SIGMA 30

edgeType calcIntensity(Color pixel)
{
    return 255 * (0.2126 * pixel.GetR() + 0.7152 * pixel.GetG() + 0.0722 * pixel.GetB());
}

indType ind(indType x, indType y, indType width)
{
    return y * width + x;
}

Graph::Graph(unsigned int numVert)
    : numVertices{numVert}
{
    nodes.resize(numVert);
}

Graph::Graph(const Bitmap &bitmap)
{   
    indType width = bitmap.Width();
    indType height = bitmap.Height();
    this->sourceS = width * height;
    this->sinkT = width * height + 1;
    nEdges(bitmap);
    tEdges(bitmap);
}
Graph::~Graph()
{
    nodes.clear();
}

void Graph::addEdge(indType start, indType end, edgeType capacity)
{
    if (start < numVertices)
        nodes[start].neighbors[end] = capacity;
    else
        std::cerr << "Node index out of bound!" << std::endl;
}

void Graph::nEdges(const Bitmap &bitmap)
{
    indType width = bitmap.Width();
    indType height = bitmap.Height();
    edgeType capacity_ = std::numeric_limits<edgeType>::min();
    for (indType x = 0; x < width; x++)
    {
        for (indType y = (x % 2 == 0) ? 0 : 1; y < height; y += 2)
        {
            if (x + 1 < width)
            {
                edgeType bp_ij_right = boundaryMetric(bitmap(x, y), bitmap(x + 1, y));
                addEdge(ind(x, y, width), ind(x + 1, y, width), bp_ij_right);
                addEdge(ind(x + 1, y, width), ind(x, y, width), bp_ij_right);
                capacity_ = std::max(capacity_, bp_ij_right);
            }
            if (y + 1 < height)
            {
                edgeType bp_ij_low = boundaryMetric(bitmap(x, y), bitmap(x, y + 1));
                addEdge(ind(x, y, width), ind(x, y + 1, width), bp_ij_low);
                addEdge(ind(x, y + 1, width), ind(x, y, width), bp_ij_low);
                capacity_ = std::max(capacity_, bp_ij_low);
            }
        }
    }
    this->capacity = capacity_;
}

void Graph::tEdges(const Bitmap& bitmap)
{
    // indType numForeground = this->seed.foreground.size();
    // indType numBackground = this->seed.background.size();
    indType width = bitmap.Width();
    indType height = bitmap.Height();
    for (indType x = 0; x < width; x++)
    {
        for(indType y = 0; y < height; y++)
        {   
            edgeType intensity = calcIntensity(bitmap(x,y));
            addEdge(ind(x, y, width),sinkT, 255 - intensity);
            addEdge(sourceS, ind(x, y, width), intensity);
        }
    }
}

edgeType boundaryMetric(Color a, Color b)
{
    Color diff = a-b;
    edgeType bd = calcIntensity(diff);
    return 100 * std::exp(-std::pow(bd,2)/(2*std::pow(SIGMA,2)));
}