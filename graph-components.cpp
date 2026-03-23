#include "graph-components.hpp"

#include <iostream>
#include <deque>
#include <string>

Graph::Graph() { };

Graph::~Graph() { };


Node& Graph::emplace_node(std::deque<int> values) {
    nodes.emplace_back(values);
    return nodes.back();
}

Edge& Graph::emplace_edge(Node* f, Node* s) {
    edges.emplace_back(f, s);
    f->add_edge( &edges.back() );
    s->add_edge( &edges.back() );
    return edges.back();
}

void Graph::erase_node(const Node& node) {
    unsigned int i = 0;
    while ( i < nodes.size() ) {
        if ( nodes[i] == node ) {
            
            for ( Edge* ptr : nodes[i].getEdges() )
                erase_edge(*ptr);

            nodes.erase(nodes.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Couldn't find node: " << &node << std::endl;
};

void Graph::erase_edge(const Edge& edge) {
    unsigned int i = 0;
    while ( i < edges.size() ) {
        if ( edges[i] == edge ) {
            edges.erase(edges.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Couldn't find node: " << &edge << std::endl;
};

std::deque<Node*> Graph::getNodePtrs() {
    std::deque<Node*> node_ptrs;

    for ( unsigned int i = 0; i < nodes.size(); i++ )
        node_ptrs.push_back( &nodes[i] );

    return node_ptrs;
};

// std::deque<Edge> Graph::getEdges() {
//     return edges;
// };

void Graph::rollcall() {
    std::cout << std::endl;
    std::cout << "=Nodes rollcall=" << std::endl;
    for ( unsigned i=0; i < nodes.size(); i++ ) {
        std::cout << & nodes[i] << ":\t";
        for ( unsigned int j=0; j < nodes[i].getEdges().size(); j++ )
            std::cout << " " << nodes[i].getEdges()[j];
        std::cout << std::endl;
    }
    std::cout << "=Edges rollcall=" << std::endl;
    for ( unsigned i=0; i < edges.size(); i++ ) {
        std::cout << & edges[i] << ":\t" << edges[i].getFirst()
            << " " << edges[i].getSecond() << std::endl;
    }
    std::cout << "=Rollcall ended=" << std::endl;
    std::cout << std::endl;
}


Node::Node(std::deque<int> values)
    : values(values) { };

Node::~Node() { }; // GRAPH IS RESPONSIBLE FOR EDGES !!!

void Node::add_edge(Edge* edge) {
    edges.push_back( edge );
};


Edge::Edge(Node* f, Node* s)
    : first(f), second(s), directional(false) { };

Edge::~Edge() { };