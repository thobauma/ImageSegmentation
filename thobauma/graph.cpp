#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <limits>

#include "graph.h"

#define SIGMA 30


indType ind(indType x, indType y, indType width)
{
    return y * width + x;
}

valueType calcIntensity(Color pixel)
{
    return 255 * (0.2126 * pixel.GetR() + 0.7152 * pixel.GetG() + 0.0722 * pixel.GetB());
}

valueType boundaryMetric(Color a, Color b)
{
    Color diff = a-b;
    valueType bd = calcIntensity(diff);
    return 100 * std::exp(-std::pow(bd,2)/(2*std::pow(SIGMA,2)));
}

// Graph::Graph(unsigned int numVert)
//     : numVertices{numVert}
// {
//     vertices.resize(numVert);
// }

Graph::Graph(const Bitmap &bitmap) : width{bitmap.Width()}, height{bitmap.Height()}
{   
    this->numVertices = width * height + 2;
    this->sourceInd = width * height;
    this->sinkInd = width * height + 1;
    vertices.reserve(numVertices);
    for (indType x = 0; x < width; x++)
    {
        for(indType y = 0; y < height; y++)
        {   
            vertices[ind(x,y,width)] = Vertex(bitmap(x,y));
        }
    }
    nEdges(bitmap);
    tEdges(bitmap);
}

Graph::~Graph()
{
    vertices.clear();
}

void Graph::addEdge(indType start, indType end, valueType capacity)
{
    if (start < numVertices)
    {
        vertices[start].neighbors[end] = Edge(capacity);
    }
    else
        std::cerr << "Node index out of bound!" << std::endl;
}

void Graph::nEdges(const Bitmap &bitmap) 
{
    // valueType capacity_ = std::numeric_limits<valueType>::min();
    for (indType x = 0; x < width; x++)
    {
        for (indType y = (x % 2 == 0) ? 0 : 1; y < height; y += 2)
        {
            if (x + 1 < width)
            {
                valueType bp_ij_right = boundaryMetric(bitmap(x, y), bitmap(x + 1, y));
                addEdge(ind(x, y, width), ind(x + 1, y, width), bp_ij_right);
                addEdge(ind(x + 1, y, width), ind(x, y, width), bp_ij_right);
                // capacity_ = std::max(capacity_, bp_ij_right);
            }
            if (y + 1 < height)
            {
                valueType bp_ij_low = boundaryMetric(bitmap(x, y), bitmap(x, y + 1));
                addEdge(ind(x, y, width), ind(x, y + 1, width), bp_ij_low);
                addEdge(ind(x, y + 1, width), ind(x, y, width), bp_ij_low);
                // capacity_ = std::max(capacity_, bp_ij_low);
            }
        }
    }
    // this->capacity = capacity_;
}

void Graph::tEdges(const Bitmap& bitmap)
{
    // indType numForeground = this->seed.foreground.size();
    // indType numBackground = this->seed.background.size();
    for (indType x = 0; x < width; x++)
    {
        for(indType y = 0; y < height; y++)
        {   
            valueType intensity = calcIntensity(bitmap(x,y));
            addEdge(ind(x, y, width),sinkInd, 255 - intensity);
            addEdge(sourceInd, ind(x, y, width), intensity);
        }
    }
}

valueType Graph::bfs(std::vector<indType>& parent)
{   
    std::queue<indType> queue;
    parent.resize(numVertices, IND_MAX);
    queue.push(sourceInd);
    while(!queue.empty())
    {   
        indType current = queue.front();
        queue.pop();
        for(auto it : vertices[current].neighbors)
        {
            indType next = it.first;
            Edge edge = it.second;
            if(parent[next] == IND_MAX && edge.residual > 0)
            {
                parent[next] = current;
                if(next == sinkInd)
                {   
                    return true;
                }
                queue.push(next);
            }
        }
    }
    return false;
}

valueType Graph::edmondsKarp()
{
    valueType maxFlow = 0;
    std::vector<indType> parent;
    while(bfs(parent))
    {
        indType current = sinkInd;
        valueType pathFlow = EDGE_MAX;
        while(current != sourceInd)
        {
            indType previous = parent[current];
            pathFlow = std::min(pathFlow, vertices[previous].neighbors[current].residual);
            current = previous;
        }
        current = sinkInd;
        while(current != sourceInd){
            indType previous = parent[current];
            vertices[previous].neighbors[current].residual -= pathFlow;
            vertices[current].neighbors[previous].residual += pathFlow;
            current = previous;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}


void Graph::minCut()
{
    valueType maxFlow = edmondsKarp();
    std::cout << "maxFlow: " << maxFlow << std::endl;
    partition(sourceInd);
    for (indType vInd = 0; vInd < numVertices; vInd++)
    {   
        Vertex v = vertices[vInd];
        if (v.visited)
        {
            for(auto it: v.neighbors)
            {
                if(!vertices[it.first].visited)
                {
                    vertices[it.first].color = Color(255/256,0,0);
                }
            }
        }
    }
}

void Graph::partition(indType vertInd)
{
    Vertex v = vertices[vertInd];
    v.visited = 1;
    for(auto it: vertices[vertInd].neighbors)
    {
        indType next = it.first;
        Edge edge = it.second;
        if(edge.residual > 0 && vertices[next].visited == 0)
        {
            partition(next);
        }
    }
}

Bitmap Graph::graphToBitmap()
{
    Bitmap result(width, height);
    for(indType y = 0; y < height; ++y)
    {
        for(indType x = 0; x < width; ++x)
        {
            result(x,y) = vertices[ind(x,y,width)].color;
        }
    }
    return result;
}