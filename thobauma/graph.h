#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

using indType = unsigned;

struct Node
{
    int color;
    std::map<indType,indType> neighbors_;

    Node() : color{0} {};
};


struct extendedNode{
    indType node, cost, from;
    bool operator<(const extendedNode& n2) const
    {
        return this->cost > n2.cost;
    }
};

class Graph
{
public:
    Graph(unsigned int numNodes);
    
    ~Graph();

    void addEdge(indType start, indType end, unsigned int capacity);

    void minCut();

private:
    std::vector<Node> nodes_;
    unsigned int numNodes_;
    unsigned int source_;
    unsigned int sink_;
};