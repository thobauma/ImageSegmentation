#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include "bitmap.hpp"

using indType = unsigned;

using valueType = double;

valueType boundaryMetric(Color a, Color b);
valueType calcIntensity(Color pixel);

struct Edge
{
    valueType capacity;
    valueType residual;
    Edge(valueType cap): capacity{cap}, residual{cap} {};
    Edge(): capacity{0}, residual{0} {};
};

struct Vertex
{   
    Color color;
    std::unordered_map<indType, Edge> neighbors;
    Vertex() : color{} {};
    Vertex(Color c) : color{c} {};
};

class Graph
{
public:

    ~Graph();

    Graph(indType n, indType s, indType t): 
        numVertices{n}, sourceInd{s}, sinkInd{t},
        vertices(n, Vertex()) {};

    Graph(const Bitmap &bitmap);

    void addEdge(indType start, indType end, valueType capacity);

    void minCut();
    
    bool bfs(std::vector<indType>& path);

    void dfsUtil(indType v, std::vector<bool>& visited);

    bool dfs(indType v, std::vector<bool>& visited);


    valueType edmondsKarp();

    Bitmap graphToBitmap();
    
    void printInfo();

/// to delete
    void printPath(std::vector<indType> parent);
    void printParent(std::vector<indType> parent);
    void printTest();

    indType getSource(){
        return sourceInd;
    }

    indType getSink(){
        return sinkInd;
    }

    indType getWidth(){
        return width;
    }

    indType getHeight(){
        return height;
    }
/// end delete

private:
    std::vector<Vertex> vertices;
    indType numVertices;
    indType width;
    indType height;
    indType sourceInd;
    indType sinkInd;

    void nEdges(const Bitmap &bitmap);
    void tEdges(const Bitmap &bitmap);
};