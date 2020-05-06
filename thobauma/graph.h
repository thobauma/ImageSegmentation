#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitmap.h>

using indType = unsigned;
using intensityType = unsigned;
using edgeType = double;
// using seedPoint = std::pair<indType, indType>;
// using seedVec = std::vector<std::pair<indType, indType>>;

edgeType boundaryMetric(Color a, Color b);
edgeType calcIntensity(Color pixel);

// struct Seed
// {
//     std::vector<std::pair<indType, indType>> foreground;
//     std::vector<std::pair<indType, indType>> background;
// };

struct Vertex
{
    intensityType intensity;
    std::unordered_map<indType, edgeType> neighbors;
    Vertex() : intensity{0} {};
};

class Graph
{
public:
    Graph(indType numVert);

    ~Graph();

    Graph::Graph(const Bitmap &bitmap);

    // void setSeed(seedVec &foreground, seedVec &background);
    void addEdge(indType start, indType end, edgeType capacity);

private:
    std::vector<Vertex> nodes;
    indType numVertices;
    indType sourceS;
    indType sinkT;
    edgeType capacity;
    // Seed seed;

    void nEdges(const Bitmap &bitmap);
    void tEdges(const Bitmap &bitmap);
};