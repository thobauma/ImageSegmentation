#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include "graph.hpp"

// #define DEBUG

#define SIGMA 30

valueType EDGE_MAX = std::numeric_limits<valueType>::max();
indType IND_MAX = std::numeric_limits<indType>::max();


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
    return 1000 * std::exp(-std::pow(bd,2)/(2*std::pow(SIGMA,2)));
}

Graph::Graph(const Bitmap &bitmap) : width{bitmap.Width()}, height{bitmap.Height()},
                numVertices{bitmap.Width() * bitmap.Height() + 2}, vertices(bitmap.Width() * bitmap.Height() + 2, Vertex()),
                sourceInd{bitmap.Width() * bitmap.Height()}, sinkInd{bitmap.Width() * bitmap.Height() + 1}
{   
    #ifdef DEBUG
        std::cout <<  "width: " << width << std::endl;
        std::cout <<  "height: " << height << std::endl;
        std::cout <<  "numVertices: " << numVertices << std::endl;
        std::cout <<  "vertices.size: " << vertices.size() << std::endl;
    #endif
    for (indType x = 0; x < width; x++)
    {
        for(indType y = 0; y < height; y++)
        {
            Vertex v(bitmap(x,y));
            vertices[ind(x,y,width)] = v;
        }
    }
    #ifdef DEBUG
        std::cout <<  "vertices.size: " << vertices.size() << std::endl;
        std::cout << "colors init" << std::endl;
    #endif
    nEdges(bitmap);
    #ifdef DEBUG
        std::cout << "nEdges done" << std::endl;
    #endif
    tEdges(bitmap);
    #ifdef DEBUG
        std::cout << "tEdges done" << std::endl;
    #endif
    
}

Graph::~Graph()
{
    vertices.clear();
}

void Graph::addEdge(indType start, indType end, valueType capacity)
{
    Edge e(capacity);
    vertices[start].neighbors.insert({end, e});
}

void Graph::nEdges(const Bitmap &bitmap) 
{
    for (indType x = 0; x < width; x++)
    {
        for (indType y = 0; y < height; y++)
        {
            if (x + 1 < width)
            {
                valueType bp_ij_right = boundaryMetric(bitmap(x, y), bitmap(x + 1, y));
                addEdge(ind(x, y, width), ind(x + 1, y, width), bp_ij_right);
                addEdge(ind(x + 1, y, width), ind(x, y, width), bp_ij_right);
            }
            if (y + 1 < height)
            {
                valueType bp_ij_low = boundaryMetric(bitmap(x, y), bitmap(x, y + 1));
                addEdge(ind(x, y, width), ind(x, y + 1, width), bp_ij_low);
                addEdge(ind(x, y + 1, width), ind(x, y, width), bp_ij_low);
            }
        }
    }
}

void Graph::tEdges(const Bitmap& bitmap)
{
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

bool Graph::bfs(std::vector<indType>& parent)
{   
    std::queue<indType> queue;
    std::fill(parent.begin(), parent.end(), IND_MAX);
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
                    #ifdef DEBUG
                        std::cout << "BFS: True" << std::endl;
                        printParent(parent);
                        printPath(parent);
                    #endif
                    return true;
                }
                queue.push(next);
            }
        }
    }
    #ifdef DEBUG
        std::cout << "BFS: False" << std::endl;
        printParent(parent);
    #endif
    return false;
}

void Graph::dfsUtil(indType v, std::vector<bool>& visited)
{
    visited[v] = 1;
    for(auto it: vertices[v].neighbors)
    {
        if(!visited[it.first])
        {
            dfsUtil(it.first, visited);
        }
    }
}
bool Graph::dfs(indType v,std::vector<bool>& visited)
{   
    dfsUtil(v, visited);
}

valueType Graph::edmondsKarp()
{   
    valueType maxFlow = 0;
    std::vector<indType> parent(numVertices);
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
        #ifdef DEBUG
            printTest();
        #endif
    }
    return maxFlow;
}

void Graph::minCut()
{
    valueType maxFlow = edmondsKarp();
    std::cout << "maxFlow: " << maxFlow << std::endl;
    std::vector<indType> parent(numVertices, IND_MAX);
    bfs(parent);
    #ifdef DEBUG
        std::cout << "partition done" << std::endl;
    #endif
    for (indType vInd = 0; vInd < numVertices; vInd++)
    {
        Vertex& v = vertices[vInd];
        if (parent[vInd] != IND_MAX)
        {
            for (auto it : v.neighbors)
            {
                if (it.first == sinkInd)
                    continue;
                if (parent[it.first] == IND_MAX)
                {   
                    #ifdef DEBUG
                        std::cout << "ind: "<< vInd << std::endl;
                    #endif
                    v.color = Color(255. / 256, 0, 0);
                }
            }
        }
    }
    #ifdef DEBUG
        std::cout << "cut done" << std::endl;
    #endif
}

bool Graph::push(Vertex v)
{

}


void Graph::relabel(Vertex v)
{

}


Bitmap Graph::graphToBitmap()
{
    Bitmap result(width, height);
    for(indType y = 0; y < height; ++y)
    {
        for(indType x = 0; x < width; ++x)
        {
            Vertex v = vertices[ind(x,y,width)];
            result(x,y) = v.color;
        }
    }
    return result;
}


void Graph::printInfo()
{
    for(indType i = 0; i < numVertices-2; i++)
    {   
        Vertex v = vertices[i];
        std::cout 
            // << "Index: " 
            << std::setw(2) << i << ": "
            <<"(" << i % width << "," << i / width << "):" 
            // << "  numEdges: " 
            << v.neighbors.size() << " ";
        std::cout
            << "  Color:"
            <<"[" << std::setw(5) << v.color.GetR()
            << "," << std::setw(5) << v.color.GetG()
            << "," << std::setw(5) << v.color.GetB() 
            << "]";
        // std::cout << "  Edges:    ";
        for(auto it: v.neighbors)
        {
            std::cout << std::setw(2) << it.first << ": "
                <<"("
                << it.first % width << "," << it.first/ width << "): "
                << std::setw(8) << it.second.capacity << ","
                << std::setw(8) << it.second.residual << "  ";
            if(it.second.residual != it.second.capacity)
                std::cout << "different! " << "   ";
        }
        std::cout << std::endl;
    }
    std::cout << "s: " << vertices[sourceInd].neighbors.size() << std::endl;
    std::cout << "t: " << vertices[sinkInd].neighbors.size() << std::endl;
}

void Graph::printTest()
{
    for(indType i = 0; i < numVertices; i++)
    {   
        Vertex v = vertices[i];
        std::cout 
            << "Index: " 
            << std::setw(2) << i << ": "
            << "  numEdges: " 
            << v.neighbors.size() << " ";
        std::cout << "  Edges:    ";
        for(auto it: v.neighbors)
        {
            std::cout << std::setw(2) << it.first << "  ";
            // << ":("
            //     << std::setw(3) << it.second.capacity << ","
            //     << std::setw(3) << it.second.residual << ")    ";
            // if(it.second.residual != it.second.capacity)
            //     std::cout << "different! " << "   ";
        }
        std::cout << std::endl;
    }
}

void Graph::printParent(std::vector<indType> parent)
{
    std::cout << "Parent: ";
    for(indType curr = sinkInd; curr > sourceInd; curr--)
    {
        std::cout << curr << ":" << parent[curr] << " ";
    }
    std::cout << std::endl;
}
void Graph::printPath(std::vector<indType> parent)
{                 
    std::cout << "Path:   ";
    indType curr = sinkInd;
    while(curr != sourceInd){
        std::cout << curr << ":"<< parent[curr] << "  ";
        curr = parent[curr];
    }
   std::cout << std::endl;
}