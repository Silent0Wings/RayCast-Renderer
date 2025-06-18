#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <vector>

class graphNode;

class graphNode
{
public:
    int value;
    std::vector<graphNode*> children;  // Pointers to child nodes
    graphNode* parent = nullptr;       // Pointer to parent node
    std::array<size_t, 2> index;

    bool blocked = false;
    bool explored = false;
    bool goal = false;


    graphNode() = default;
};

#endif // GRAPHNODE_H
