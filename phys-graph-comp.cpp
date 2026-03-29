#include "phys-graph-comp.hpp"

#include "graph-components.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

PhysSpace::PhysSpace(Graph& graph) : graph( &graph ) {
    for ( Node* tmp : this->graph->getNodes() ) {
        nodes.emplace_back(tmp);
        std::cout << nodes.back().getNode() << std::endl;
    }
    std::cout << std::endl;
};

PhysSpace::~PhysSpace() { };

// void PhysSpace::update_all() {
//     for ( unsigned int i=0; i < nodes.size(); i++ )
//         nodes[i].update_velocity();
//     for ( unsigned int i=0; i < nodes.size(); i++ )
//         nodes[i].update_coord();
// }

// void PhysSpace::display_all() { };

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