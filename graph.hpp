#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include "bitmap.hpp"

using indType = unsigned;
using valueType = double;
using excessContainer = std::queue<indType>;

valueType boundaryMetric(Color a, Color b);
valueType calcIntensity(Color pixel);

struct Edge
{
    valueType capacity;
    valueType residual;
    valueType flow;
    Edge(valueType cap): 
            capacity{cap}, residual{cap}, flow{0} {};
    Edge(valueType cap, valueType f): 
            capacity{cap}, residual{cap}, flow{f} {};
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



class Graph
{
public:
    ~Graph();

    Graph(indType n, indType s, indType t): 
        vertices(n, Vertex()), numVertices{n},
        width{0}, height{0},
        sourceInd{s}, sinkInd{t}
        {};

    Graph(const Bitmap &bitmap);

    void addEdge(indType start, indType end, 
                valueType capacity, valueType flow);

    bool bfs(std::vector<indType>& path);

    valueType edmondsKarp();

    void minCut();
    
    void push(indType uInd, indType vInd, excessContainer& excessVertices);

    void relabel(indType uInd);

    void discharge(indType uInd, excessContainer& excessVertices);

    valueType prMaxFlow();

    bool prBFS(std::vector<indType>& parent);

    void prMinCut();

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