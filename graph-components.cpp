#include "graph-components.hpp"

#include <deque>
#include <vector>
#include <map>

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

std::vector<Node*> Graph::getNodes() {
    std::vector<Node*> node_ptrs;

    for ( unsigned int i = 0; i < nodes.size(); i++ )
        node_ptrs.push_back( &nodes[i] );

    return node_ptrs;
};

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
    : values(values), coords({0, 0, 0}), checked(false) { };

Node::Node(std::vector<int> values, xyz coords)
    : values(values), coords(coords), checked(false) { };

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

// Physics

void Graph::update_nodes() {

    // temporary nonsence
    nodes[ (unsigned)( xyz_rnd_direction(4).x + 4) ].update_coords();

};

void Graph::display_node(sf::RenderWindow& window, float scale, xyz point) {

    float RadiusInPixels = 0.06 * scale;

    unsigned int br = brightness(point.z);
    sf::Color color = sf::Color(255 - br, 255 - br, br);

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);

    circle.setPosition(point.x, point.y);
    window.draw(circle);
};

void Graph::display_edge(sf::RenderWindow& window, float scale, xyz p1, xyz p2) {

    scale *= 2; // rest, -Werror=unused-parameter

    unsigned int br1 = brightness(p1.z);
    unsigned int br2 = brightness(p2.z);

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.x, p1.y)),
        sf::Vertex(sf::Vector2f(p2.x, p2.y))
    };

    line[0].color = sf::Color(255 - br1, 255 - br2, br1);
    line[1].color = sf::Color(255 - br1, 255 - br2, br2);

    window.draw(line, 2, sf::Lines);
};

xyz Graph::calc_node_screen_coords(const Node& node) {
    
    return node.getCoords();

};

void Graph::display(sf::RenderWindow& window, float scale) {

    unsigned int window_width = window.getSize().x;
    unsigned int window_height = window.getSize().y;
    unsigned int number_of_nodes = nodes.size();

    std::map<Node*, xyz> nodes_xyzs;

    for ( unsigned int i = 0; i < number_of_nodes; i++ ) {

        xyz node_scr_c = calc_node_screen_coords( nodes[i] );
        node_scr_c.x = node_scr_c.x * scale + window_width / 2;
        node_scr_c.y = node_scr_c.y * scale + window_height / 2;
        node_scr_c.z = node_scr_c.z * scale;

        nodes_xyzs[ &nodes[i] ] = node_scr_c;
    }

    for ( unsigned int i = 0; i < number_of_nodes; i++ ) {
        display_node( window, scale, nodes_xyzs[ &nodes[i] ] );

        std::vector<Node*> neighbours = nodes[i].getEdges();
        for ( unsigned int j = 0; j < neighbours.size(); j++ )
            display_edge( window, scale, nodes_xyzs[ &nodes[i] ], nodes_xyzs[ neighbours[j] ] );
    }

};

void Node::update_coords() {

    coords = xyz_rnd_direction(2.f);
    
};