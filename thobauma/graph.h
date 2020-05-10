#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <string>
#include "bitmap.h"

using indType = unsigned;
using intensityType = unsigned;
using valueType = double;
using edgeType = std::pair<valueType,valueType>;
valueType EDGE_MAX = std::numeric_limits<valueType>::max();
indType IND_MAX = std::numeric_limits<indType>::max();
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
};

struct Vertex
{   
    Color color;
    bool visited;
    intensityType intensity;
    std::map<indType, Edge> neighbors;
    Vertex() : intensity{0}, visited{0}, color{} {}
    Vertex(Color c) : intensity{calcIntensity(c)} ,visited{0}, color{c} {}
};

class Graph
{
public:
    // Graph(indType numVert);

    ~Graph();

    Graph::Graph(const Bitmap &bitmap);

    // void setSeed(seedVec &foreground, seedVec &background);
    void addEdge(indType start, indType end, valueType capacity);

    void minCut();
    
    valueType Graph::bfs(std::vector<indType>& path);

    valueType edmondsKarp();

    void partition(indType vertInd);

    Bitmap graphToBitmap();

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