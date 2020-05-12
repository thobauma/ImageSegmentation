#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include "bitmap.hpp"

using indType = unsigned;
// using intensityType = unsigned;
using valueType = double;
// using edgeType = std::pair<valueType,valueType>;




// using seedPoint = std::pair<indType, indType>;
// using seedVec = std::vector<std::pair<indType, indType>>;

valueType boundaryMetric(Color a, Color b);
valueType calcIntensity(Color pixel);

// struct Seed
// {
//     std::vector<std::pair<indType, indType>> foreground;
//     std::vector<std::pair<indType, indType>> background;
// };

struct Edge
{
    valueType capacity;
    valueType residual;
    Edge(valueType cap): capacity{cap}, residual{cap} {};
    Edge(): capacity{0}, residual{0} {};
    // Edge& operator=(Edge& e) noexcept
    // {
    //     std::swap(capacity, e.capacity);
    //     std::swap(residual, e.residual);
    //     return *this;
    // }
};

struct Vertex
{   
    Color color;
    valueType intensity;
    bool visited;
    std::unordered_map<indType, Edge> neighbors;
    Vertex() : color{}, intensity{0}, visited{0} {}
    Vertex(Color c) : color{c}, intensity{calcIntensity(c)} ,visited{0} {}
};

class Graph
{
public:
    // Graph(indType numVert);

    ~Graph();

    Graph(indType n, indType s, indType t): 
        numVertices{n}, sourceInd{s}, sinkInd{t},
        vertices(n, Vertex()) {};

    Graph(const Bitmap &bitmap);

    // void setSeed(seedVec &foreground, seedVec &background);
    void addEdge(indType start, indType end, valueType capacity);

    void minCut();
    
    valueType bfs(std::vector<indType>& path);

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

/// end delete

private:
    std::vector<Vertex> vertices;
    indType numVertices;
    indType width;
    indType height;
    indType sourceInd;
    indType sinkInd;
    // valueType capacity;
    // Seed seed;

    void nEdges(const Bitmap &bitmap);
    void tEdges(const Bitmap &bitmap);
};