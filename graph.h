#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>
#include <limits>
#include "Node.h"
#include "Edge.h"

const int INF = std::numeric_limits<int>::max();

class Graph {
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    bool directed;

    int indexOf(const std::string& name) const;

public:
    Graph(const std::vector<std::vector<int> >& matrix);

    void addEdge(const std::string& src, const std::string& dst, int weight = 1);

    bool isMultigraph() const;
    bool isPseudograph() const;
    bool isDigraph() const;
    bool isWeighted() const;
    bool isComplete() const;
    bool isDisjointed() const;
    std::vector<std::string> isolatedNodes() const;

    void report() const;

    std::vector<int> dijkstra(const std::string& startName, std::vector<int>& prev) const;
    std::string pathTo(int target, const std::vector<int>& prev) const;
    void printShortestPaths(const std::string& startName) const;
};

#endif