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
    valueType flow;
    Edge(valueType cap): capacity{cap}, residual{cap}, flow{0} {};
    Edge(): capacity{0}, residual{0}, flow{0} {};
};

struct Vertex
{   
    Color color;
    indType vertexHeight;
    valueType excessFlow;
    std::unordered_map<indType, Edge> neighbors;
    Vertex() : color{}, vertexHeight{0}, excessFlow{0} {};
    Vertex(Color c) : color{c}, vertexHeight{0}, excessFlow{0} {};
};

class prGraph
{
public:

    ~prGraph();

    prGraph(indType n, indType s, indType t): 
        vertices(n, Vertex()), numVertices{n},
        width{0}, height{0},
        sourceInd{s}, sinkInd{t}
        {};

    prGraph(const Bitmap &bitmap);

    void addEdge(indType start, indType end, valueType capacity);

    bool bfs(std::vector<indType>& path);

    void dfsUtil(indType v, std::vector<bool>& visited);

    bool dfs(indType v, std::vector<bool>& visited);

    valueType edmondsKarp();

    void minCut();

    bool push(Vertex v);

    void relabel(Vertex v);



    Bitmap graphToBitmap();

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
    
    void printInfo();
    void printPath(std::vector<indType> parent);
    void printParent(std::vector<indType> parent);
    void printTest();

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