#include "graph-components.hpp"

#include <deque>
#include <vector>
#include <string>

#include <iostream>

Graph::Graph() : allow_equal_nodes(false), allow_multiple_edges(false) { };

Graph::~Graph() { };


Node* Graph::emplace_node(std::vector<int> values) {
    Node tmp_node(values);

    if ( allow_equal_nodes ) {
        nodes.push_back(tmp_node);
        return &( nodes.back() );
    }
    
    unsigned int i = 0;
    while ( i < nodes.size() ) {
        if ( nodes[i] == tmp_node ) {
            std::cout << "(!) Attempted to double node" << std::endl;
            return nullptr;
        }
        else
            i++;
    }
    nodes.push_back(tmp_node);
    return &( nodes.back() );
}

Edge* Graph::emplace_edge(Node* f, Node* s) {
    Edge tmp_edge(f, s);

    if ( allow_multiple_edges ) {
        edges.push_back(tmp_edge);
        f->add_edge( &edges.back() );
        s->add_edge( &edges.back() );
        return &( edges.back() );
    }

    unsigned int i = 0;
    while ( i < edges.size() ) {
        if ( edges[i] == tmp_edge ) {
            std::cout << "(!) Attempted to double edge" << std::endl;
            return nullptr;
        }
        else
            i++;
    }
    edges.push_back(tmp_edge);
    f->add_edge( &edges.back() );
    s->add_edge( &edges.back() );
    return &( edges.back() );
}

void Graph::erase_node(const Node& node) {
    unsigned int i = 0;
    while ( i < nodes.size() ) {

        if ( nodes[i] == node ) {
            for ( Edge* edge : nodes[i].getEdges() )
                erase_edge(*edge);
            nodes.erase(nodes.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Graph couldn't erase node" << std::endl;
};

void Graph::erase_edge(const Edge& edge) {
    unsigned int i = 0;
    while ( i < edges.size() ) {

        if ( edges[i] == edge ) {
            edges[i].getFirst()->remove_edge(edge);
            edges[i].getSecond()->remove_edge(edge);
            edges.erase(edges.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Graph couldn't erase edge" << std::endl;
};

std::vector<Node*> Graph::getNodes() {
    std::vector<Node*> node_ptrs;

    for ( unsigned int i = 0; i < nodes.size(); i++ )
        node_ptrs.push_back( &nodes[i] );

    return node_ptrs;
};

std::vector<Edge*> Graph::getEdges() {
    std::vector<Edge*> edge_ptrs;

    for ( unsigned int i = 0; i < edges.size(); i++ )
        edge_ptrs.push_back( &edges[i] );

    return edge_ptrs;
};

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


Node::Node(std::vector<int> values)
    : values(values) { };

Node::~Node() { };

Edge* Node::add_edge(Edge* edge) {
    edges.push_back( edge );
    return edges.back();
};

void Node::remove_edge(const Edge& edge) {
    unsigned int i = 0;
    while ( i < edges.size() ) {

        if ( *edges[i] == edge ) {
            edges.erase(edges.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Node couldn't remove edge" << std::endl;
}


Edge::Edge(Node* f, Node* s)
    : first(f), second(s), directional(false) { };

Edge::~Edge() { };