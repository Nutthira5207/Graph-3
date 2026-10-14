#ifndef NODE_H
#define NODE_H

#include <string>

class Node {
public:
    std::string name;

    Node(const std::string& n) : name(n) {}
};

#endif