#include "phys-graph-comp.hpp"

#include "graph-components.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

void xyz::display() {
    std::cout << x << " " << y << " " << z << std::endl;
}

xyz rnd_direction() {
    std::random_device rd; 
    std::mt19937 engine( rd() ); 
    std::uniform_int_distribution<int> dist(-1000, 1000);

    // I'm sorry (not spherically symmetrical)
    xyz dir;
    dir = {
        0.001*dist(engine),
        0.001*dist(engine),
        0.001*dist(engine)
    };

    // I'm so sorry
    if ( !dir.x && !dir.y && !dir.z )
        dir.x = 1;

    return dir;
}

PhysSpace::PhysSpace(Graph& graph) : graph( &graph ) {
    sync_nodes();
};

PhysSpace::~PhysSpace() { };

void PhysSpace::sync_nodes() {
    std::vector<PhysNode> synced;

    for ( Node* tmp : this->graph->getNodes() )
        synced.emplace_back(tmp, rnd_direction());

    unsigned int len = nodes.size();
    if ( len > synced.size() )
        len = synced.size();
    
    for ( unsigned int i=0; i < len; i++ )
        synced[i].coords = nodes[i].coords;
    
    nodes = synced;
};

// void PhysSpace::update_all() {
//     for ( unsigned int i=0; i < nodes.size(); i++ )
//         nodes[i].update_velocity();
//     for ( unsigned int i=0; i < nodes.size(); i++ )
//         nodes[i].update_coord();
// }

// void PhysSpace::display_all() { };

// void PhysSpace::manual_add(xyz coords) {
//     nodes.emplace_back(nullptr, rnd_direction);
// }

PhysNode::PhysNode(Node* node, xyz coords) :
    node(node), coords(coords), velocity({0, 0, 0}) { };

PhysNode::PhysNode(Node* node) :
    node(node), coords({0, 0, 0}), velocity({0, 0, 0}) { };

PhysNode::~PhysNode() { };

// void PhysNode::update_velocity() { };

// void PhysNode::update_coord() { };

xyz PhysNode::calculate_projection() {
    return {0, 0, 0};
};