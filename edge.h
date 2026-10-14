#ifndef EDGE_H
#define EDGE_H
#include <utility>
#include <algorithm>

class Edge {
public:
    int source;
    int target;
    int weight;
    bool directed;

    Edge(int s, int t, int w, bool d)
        : source(s), target(t), weight(w), directed(d) {}

    std::pair<int, int> key() const {
        if (directed)
            return std::make_pair(source, target);
        return std::make_pair(std::min(source, target), std::max(source, target));
    }
};

#endif