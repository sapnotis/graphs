#include "graph-components.hpp"

#include <deque>
#include <vector>

#include <iostream>
#include <SFML/Graphics.hpp>

Graph::Graph() : allow_equal_nodes(false), allow_multiple_edges(false) { };

Graph::~Graph() { };

Node* Graph::emplace_node(std::vector<int> values) {
    if ( allow_equal_nodes ) {
        nodes.emplace_back(values);
        return &( nodes.back() );
    }
    
    if ( findNode(values) ) {
        std::cout << "(!) Graph attempted to double node" << std::endl;
        return nullptr;
    }
    
    nodes.emplace_back(values);
    return &( nodes.back() );
}

void Graph::emplace_edge(std::vector<int> val_f, std::vector<int> val_s) {
    Node* f = findNode(val_f);
    Node* s = findNode(val_s);
    emplace_edge( f, s );
};

void Graph::emplace_edge(Node* f, std::vector<int> val_s) {
    Node* s = findNode(val_s);
    emplace_edge( f, s );
};

void Graph::emplace_edge(std::vector<int> val_f, Node* s) {
    Node* f = findNode(val_f);
    emplace_edge( f, s );
};

void Graph::emplace_edge(Node* f, Node* s) {
    if ( !f || !s ) {
        std::cout << "(!) Graph attepted to emplace nullptr edge" << std::endl;
        return;
    }

    if ( f==s ) {
        std::cout << "(!) Graph attepted to loop new edge" << std::endl;
        return;
    }

    if ( allow_multiple_edges ) {
        f->add_edge( s );
        s->add_edge( f );
        return;
    }

    if ( f->find_edge(s) || s->find_edge(f) ) {
        std::cout << "(!) Graph attempted to double edge" << std::endl;
        return;
    }

    f->add_edge( s );
    s->add_edge( f );
}

void Graph::erase_node(Node* node) {
    erase_node( *node );
};

void Graph::erase_node(const Node& node) {
    unsigned int i = 0;
    unsigned int len = nodes.size();

    while ( i < len ) {

        if ( nodes[i] == node ) {
            for ( Node* tmp : nodes[i].getEdges() )
                tmp->forget_edge( &nodes[i] );
            nodes.erase(nodes.begin() + i);
            return;
        }
        else
            i++;
    }
    
    std::cout << "(!) Graph couldn't erase node" << std::endl;
};

void Graph::erase_edge(std::vector<int> val_f, std::vector<int> val_s) {
    Node* f = findNode(val_f);
    Node* s = findNode(val_s);
    erase_edge( f, s );
};

void Graph::erase_edge(Node* f, Node* s) {
    if ( ! f->find_edge( s ) || ! s->find_edge( f ) ) {
        std::cout << "(!) Graph couldn't erase edge" << std::endl;
    }

    f->forget_edge( s );
    s->forget_edge( f );
};

std::vector<Node*> Graph::getNodes() {
    std::vector<Node*> node_ptrs;

    for ( unsigned int i = 0; i < nodes.size(); i++ )
        node_ptrs.push_back( &nodes[i] );

    return node_ptrs;
};

Node* Graph::findNode(std::vector<int> values) {
    Node tmp_node(values);

    unsigned int i = 0;
    while ( i < nodes.size() ) {
        if ( nodes[i] == tmp_node )
            return &( nodes[i] );
        else
            i++;
    }
    return nullptr;
}

void Graph::rollcall() {
    std::cout << std::endl;
    std::cout << "=Nodes rollcall=" << std::endl;
    for ( unsigned i=0; i < nodes.size(); i++ ) {
        std::cout << & nodes[i];
        std::cout << "\t";
        for ( unsigned int j=0; j < nodes[i].getValues().size(); j++ )
            std::cout << " " << nodes[i].getValues()[j];
        std::cout << "\t";
        for ( unsigned int j=0; j < nodes[i].getEdges().size(); j++ )
            std::cout << " " << nodes[i].getEdges()[j];
        std::cout << std::endl;
    }
    std::cout << "=Rollcall ended=" << std::endl;
    std::cout << std::endl;
}


Node::Node(std::vector<int> values)
    : values(values), coords({0, 0, 0}), screen_coords({0, 0, 0}), checked(false) { };

Node::Node(std::vector<int> values, xyz coords)
    : values(values), coords(coords), screen_coords({0, 0, 0}), checked(false) { };

Node::~Node() { };

void Node::add_edge(Node* node) {
    edges.push_back(node);
};

bool Node::find_edge(Node* node) {
    unsigned int i = 0;
    while ( i < edges.size() ) {

        if ( edges[i] == node )
            return true;
        else
            i++;
    }
    return false;
};

void Node::forget_edge(Node* node) {
    unsigned int i = 0;
    while ( i < edges.size() ) {

        if ( edges[i] == node ) {
            edges.erase(edges.begin() + i);
            return;
        }
        else
            i++;
    }
    std::cout << "(!) Node couldn't forget edge" << std::endl;
};

// SFML

void Graph::update_nodes() {

    unsigned int len = nodes.size();
    for ( unsigned int i=0; i < len; i++ ) {
        nodes[i].update_coords();
        nodes[i].update_screen_coords();
    }

};

// void Graph::update_observer_goal();
// void Graph::update_observer();

void Graph::display(sf::RenderWindow& window, float scale) {

    // unsigned int len = nodes.size();
    // for ( unsigned int i=0; i < len; i++ ) {
    //     nodes[i].display_self(window, scale);
    //     nodes[i].display_edges(window, scale);
    // }

    for ( Node tmp : nodes ) {
        tmp.display_self(window, scale);
        tmp.display_edges(window, scale);
    }

};

void Node::update_coords() {

    coords = xyz_rnd_direction(2);
    
};

void Node::update_screen_coords() {

    screen_coords = coords;

};

void Node::display_self(sf::RenderWindow& window, float scale) {

    float RadiusInPixels = 0.06 * scale;
    sf::Color color = sf::Color::White;

    if ( this->checked )
        color = sf::Color(100, 100, 100);

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);
    
    // circle.setOutlineColor(sf::Color::White);
    // circle.setOutlineThickness(1.f);

    xy projection = {screen_coords.x, screen_coords.y};
    xy dispCoords = window_xy(projection, scale);

    circle.setPosition(dispCoords.x, dispCoords.y);
    window.draw(circle);
};

void Node::display_edges(sf::RenderWindow& window, float scale) {

    xy self_projection = {screen_coords.x, screen_coords.y};
    xy self_dispCoords = window_xy(self_projection, scale);

    for ( Node* tmp : edges ) {

        xy next_projection = {tmp->getScreenCoords().x, tmp->getScreenCoords().y};
        xy next_dispCoords = window_xy( next_projection, scale);

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