#include "graph-components.hpp"

#include <deque>
#include <vector>
#include <map>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>

Graph::Graph() : allow_equal_nodes(false), allow_multiple_edges(false),
    center({0, 0, 0}), small_corner({0, 0, 0}), big_corner({0, 0, 0}),
    yaw(0), pitch(0) { };

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
    : values(values), coords(xyz_rnd_direction(1.f)), velocity({0, 0, 0}), checked(false) { };

Node::Node(std::vector<int> values, xyz coords)
    : values(values), coords(coords), velocity({0, 0, 0}), checked(false) { };

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

// V3 and SFML

void Graph::update_nodes() {
    
    float r = 0.004;
    float k = 0.004;
    unsigned int number_of_nodes = nodes.size();
    
    // O(N^2) alarm
    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        for ( unsigned int j = i+1; j < number_of_nodes; j++ ) {
            xyz delta_xyz = nodes[j].getCoords() - nodes[i].getCoords();
            nodes[j].add_velocity( delta_xyz * ( r / dist_squared(delta_xyz) ) );
            nodes[i].add_velocity( delta_xyz * ( -r / dist_squared(delta_xyz) ) );
        }
    
    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        for ( Node* neighbour : nodes[i].getEdges() ) {
            xyz delta_xyz = neighbour->getCoords() - nodes[i].getCoords();
            neighbour->add_velocity( delta_xyz * ( -k * dist_squared(delta_xyz) ) );
            nodes[i].add_velocity( delta_xyz * ( k * dist_squared(delta_xyz) ) );
        }
    // O(N^2) alarm end
    
    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        nodes[i].update_coords();
};

void Graph::display(sf::RenderWindow& window, float scale) {

    if ( yaw > 6.28f ) yaw -= 6.28f;
    if ( yaw < -6.28f ) yaw += 6.28f;

    if ( pitch > 1.2f ) pitch = 1.2f;
    if ( pitch < -1.2f ) pitch = -1.2f;
    
    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };
    unsigned int number_of_nodes = nodes.size();
    


    small_corner = big_corner = {0, 0, 0};

    for ( Node tmp : nodes ) {

        xyz tmp_coords = tmp.getCoords();

        if ( tmp_coords.x < small_corner.x )
            small_corner.x = tmp_coords.x;
        if ( tmp_coords.y < small_corner.y )
            small_corner.y = tmp_coords.y;
        if ( tmp_coords.z < small_corner.z )
            small_corner.z = tmp_coords.z;
        
        if ( tmp_coords.x > big_corner.x )
            big_corner.x = tmp_coords.x;
        if ( tmp_coords.y > big_corner.y )
            big_corner.y = tmp_coords.y;
        if ( tmp_coords.z > big_corner.z )
            big_corner.z = tmp_coords.z;
    }

    small_corner -= {0.2f, 0.2f, 0.2f};
    big_corner += {0.2f, 0.2f, 0.2f};

    center = {0, 0, 0};
    center += (small_corner * 0.5f);
    center += (big_corner * 0.5f);

    display_grid( window, scale );



    std::map<Node*, xyz> nodes_window_coords;

    for ( unsigned int i = 0; i < number_of_nodes; i++ ) {
        nodes_window_coords[ &nodes[i] ] =
            calc_window_coords( nodes[i].getCoords(), scale, window_center );
    }

    for ( unsigned int i = 0; i < number_of_nodes; i++ ) {

        display_node( window, nodes_window_coords[ &nodes[i] ] );

        std::vector<Node*> neighbours = nodes[i].getEdges();
        for ( unsigned int j = 0; j < neighbours.size(); j++ )
            display_edge( window, nodes_window_coords[ &nodes[i] ], nodes_window_coords[ neighbours[j] ] );
    }

};

void Graph::display_point(sf::RenderWindow& window, sf::Vector2f point, float RadiusInPixels, sf::Color color) {
    
    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);
    circle.setPosition(point);
    window.draw(circle);
};

void Graph::display_line(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Color color) {
    display_line( window, p1, p2, color, color );
};

void Graph::display_line(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Color col1, sf::Color col2) {

    sf::Vertex line[] = { sf::Vertex(p1), sf::Vertex(p2) };
    line[0].color = col1;
    line[1].color = col2;
    window.draw(line, 2, sf::Lines);
};

xyz Graph::calc_window_coords(xyz coords, float scale, sf::Vector2f window_center) {

    coords -= center;

    xyz yaw_coords = {
        coords.x * std::cos( yaw ) - coords.z * std::sin( yaw ),
        coords.y,
        coords.x * std::sin( yaw ) + coords.z * std::cos( yaw )
    };

    xyz final_coords = {
        yaw_coords.x,
        yaw_coords.y * std::cos( pitch ) - yaw_coords.z * std::sin( pitch ),
        yaw_coords.y * std::sin( pitch ) + yaw_coords.z * std::cos( pitch )
    };

    // perspective?
    final_coords.x *= ( 10 / ( 10 - final_coords.z )  );
    final_coords.y *= ( 10 / ( 10 - final_coords.z )  );

    final_coords.x = final_coords.x * scale + window_center.x;
    final_coords.y = final_coords.y * scale + window_center.y;
    final_coords.z = final_coords.z * scale;

    return final_coords;

};

void Graph::display_node(sf::RenderWindow& window, xyz coords) {

    sf::Vector2f point = {coords.x, coords.y};
    display_point( window, point, 5, sf::Color::White );

};

void Graph::display_edge(sf::RenderWindow& window, xyz c1, xyz c2) {

    sf::Vector2f p1 = {c1.x, c1.y};
    sf::Vector2f p2 = {c2.x, c2.y};
    display_line( window, p1, p2, sf::Color::White );

};

void Graph::display_grid(sf::RenderWindow& window, float scale) {

    sf::Color grid_color = sf::Color::Blue;

    std::map<int, sf::Vector2f> corners;

    // big brain bool logic
    for ( int i = 0; i < 8; i++ ) {
        xyz corner_window_coords = calc_window_coords(
            { (i % 2) * small_corner.x + !(i % 2) * big_corner.x,
            (i/2 % 2) * small_corner.y + !(i/2 % 2) * big_corner.y,
            (i/4 % 8) * small_corner.z + !(i/4 % 2) * big_corner.z },
            scale,
            { 0.5f * window.getSize().x,
            0.5f * window.getSize().y } );
            
        corners[i] = {corner_window_coords.x, corner_window_coords.y};
    }
    
    for ( int i = 0; i < 4; i ++ )
        display_line( window, corners[i], corners[i+4], grid_color );
    for ( int i = 0; i < 4; i ++ )
        display_line( window, corners[2*i], corners[2*i+1], grid_color );
    for ( int i = 0; i < 2; i ++ )
        display_line( window, corners[i], corners[i+2], grid_color );
    for ( int i = 4; i < 6; i ++ )
        display_line( window, corners[i], corners[i+2], grid_color );

    xyz center_window_coords = calc_window_coords(
        center, scale,
        { 0.5f * window.getSize().x,
        0.5f * window.getSize().y } );
    display_point( window, {center_window_coords.x, center_window_coords.y}, 3, grid_color );
}

void Node::update_coords() {

    coords += velocity;
    reset_velocity();
    
};