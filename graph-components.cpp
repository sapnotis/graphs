#include "graph-components.hpp"

#include <iostream>
#include <vector>
#include <string>

Graph::Graph() {
    // nodes.reserve(512);
    // edges.reserve(512);
    //      doesn't work because nodes from the middle may be removed
    //      and cause other to shift
};

Graph::~Graph() { };


Node& Graph::emplace_node(std::vector<int> values) {
    nodes.emplace_back(values);
    return nodes.back();
}

// Edge& Graph::emplace_edge(Node& f, Node& s) {
//     edges.emplace_back(&f, &s);
//     return edges.back();
// }

void Graph::erase_node(const Node& node) {
    unsigned int i = 0;
    while ( i < nodes.size() ) {
        if ( nodes[i] == node ) {
            nodes.erase(nodes.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Couldn't find node: " << &node << std::endl;
};

// void Graph::erase_edge(const Edge& edge) {
//     unsigned int i = 0;
//     while ( i < edges.size() ) {
//         if ( edges[i] == edge ) {
//             edges.erase(edges.begin() + i);
//             return;
//         }
//         else
//             i++;
//     }
//     std::cout << "(!) Couldn't find edge: " << &edge << std::endl;
// };

std::vector<Node> Graph::getNodes() {
    return nodes;
};

std::vector<Edge> Graph::getEdges() {
    return edges;
};

void Graph::rollcall() {
    std::cout << "==Nodes rollcall==" << std::endl;
    for ( unsigned i=0; i < nodes.size(); i++ ) {
        std::cout << & nodes[i] << ":\t";
        for ( unsigned int j=0; j < nodes[i].getValues().size(); j++ )
            std::cout << " " << nodes[i].getValues()[j];
        std::cout << std::endl;
    }
    std::cout << "==Edges rollcall==" << std::endl;
    for ( unsigned i=0; i < edges.size(); i++ ) {
        std::cout << & edges[i] << ":\t" << edges[i].getFirst()
            << " " << edges[i].getSecond() << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


Node::Node(std::vector<int> values)
    : values(values) { };

Node::~Node() {
    for ( Edge* elem : edges )
        delete elem;
};

bool Node::operator==(const Node& other) const {
    return ( values == other.values );
}


Edge::Edge(Node* f, Node* s)
    : first(f), second(s), directional(false) { };

Edge::~Edge() { };

bool Edge::operator==(const Edge& other) const {
    return ( first == other.first && second == other.second );
}