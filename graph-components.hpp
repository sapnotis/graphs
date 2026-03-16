#ifndef _GRAPH_LOGIC_
#define _GRAPH_LOGIC_

#include <vector>

class Graph;
class Node;
class Edge;

class Graph
{
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
public:
    Graph(); // no argumnets probably
    ~Graph(); // destroy all belonging nodes
    bool is_corrupted(); // full debug: all objects know their connections, no edges lead away
    void add_node(Node* node);
    void add_edge(Node* node1, Node* node2);
};

#endif