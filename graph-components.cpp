#include "graph-components.hpp"

#include <iostream>
#include <vector>
#include <string>

Graph::Graph() { };
Graph::~Graph() { };

void Graph::push_node(const Node& node) {
    nodes.push_back(node);
}

void Graph::push_edge(const Edge& edge) {
    edges.push_back(edge);
}

void Graph::erase_node(const Node& node) {
} // ...

void Graph::erase_edge(const Edge& edge) {
} // ...

void Graph::rollcall() {
    for ( unsigned i=0; i < nodes.size(); i++ ) {
        std::cout << nodes[i].getName() << ":\t";
        for ( unsigned int j=0; j < nodes[i].getValues().size(); j++ )
            std::cout << " " << nodes[i].getValues()[j];
        std::cout << std::endl;
    }
}

Node::Node(std::string name, std::vector<int> values)
    : name(name), values(values) { };

Node::~Node() { };

Edge::Edge(Node* f, Node* s)
    : first(f), second(s), directional(false) { };

Edge::~Edge() { };