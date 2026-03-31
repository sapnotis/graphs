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

void xy::display() {
    std::cout << x << " " << y << std::endl;
}

xyz xyz_rnd_direction() {
    std::random_device rd; 
    std::mt19937 engine( rd() ); 
    std::uniform_int_distribution<int> dist(-1000, 1000);

    // I'm sorry (not spherically symmetrical)
    xyz dir;
    dir = {
        0.001f * dist(engine),
        0.001f * dist(engine),
        0.001f * dist(engine)
    };

    // I'm so sorry
    if ( !dir.x && !dir.y && !dir.z )
        dir.x = 1;

    return dir;
}

xy window_xy(xy coords, sf::RenderWindow& window, float scale) {
	xy disp;
    disp.x = window.getSize().x / 2 + scale * coords.x;
    disp.y = window.getSize().y / 2 + scale * coords.y;
	return disp;
};

PhysSpace::PhysSpace(Graph& graph) : graph( &graph ) {
    sync_nodes();
};

PhysSpace::~PhysSpace() { };

void PhysSpace::sync_nodes() {
    std::vector<PhysNode> synced;

    for ( Node* tmp : this->graph->getNodes() )
        synced.emplace_back(tmp);

    unsigned int len = nodes.size();
    if ( len > synced.size() )
        len = synced.size();
    
    for ( unsigned int i=0; i < len; i++ )
        synced[i].coords = nodes[i].coords;
    
    nodes = synced;
};

// void PhysSpace::update_all()

void PhysSpace::display_all(sf::RenderWindow& window, float scale) {
    for ( PhysNode tmp : nodes ) {
        tmp.display_self(window, scale);
        tmp.display_edges(window, scale);
    }
};

PhysNode::PhysNode(Node* node, xyz coords) :
    coords(coords), projection({0, 0}) { };

PhysNode::PhysNode(Node* node) :
    coords({0, 0, 0}), projection({0, 0}) { };

PhysNode::~PhysNode() { };

void PhysNode::update_coords() {

};

void PhysNode::update_projection() {
    projection = {coords.x, coords.y};
};

void PhysNode::display_self(sf::RenderWindow& window, float scale) {

    float RadiusInPixels = 0.1 * scale;

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(sf::Color::White);
    
    // circle.setOutlineColor(sf::Color::White);
    // circle.setOutlineThickness(1.f);

    xy dispCoords = window_xy(projection, window, scale);

    circle.setPosition(dispCoords.x, dispCoords.y);
    window.draw(circle);
};

void PhysNode::display_edges(sf::RenderWindow& window, float scale) {

    xy self_dispCoords = window_xy(projection, window, scale);

    // need to manage all neighbours somehow
    // seems like all Graph's logic slowly duplicates
    // seems like Graph and PhysSpace can't be separated
    
    for ( ... ) {

        xy next_dispCoords = window_xy( ... , window, scale);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(self_dispCoords.x, self_dispCoords.y)),
            sf::Vertex(sf::Vector2f(next_dispCoords.x, next_dispCoords.y))
        };

        line[0].color = sf::Color::White;
        line[1].color = sf::Color::White;

        window.draw(line, 2, sf::Lines);
    }
};