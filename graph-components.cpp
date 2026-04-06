#include "graph-components.hpp"

#include <deque>
#include <vector>
#include <map>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>

Graph::Graph() : allow_equal_nodes(false), allow_multiple_edges(false),
    POV({0, 0, 0}), small_corner({0, 0, 0}), big_corner({0, 0, 0}),
    yaw(0), pitch(0), closest_farthest_z({0, 0}) { };

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

    xyz center_mass = {0, 0, 0};
    for ( Node tmp : nodes )
        center_mass += tmp.getCoords();
    
    center_mass *= ( -1.f / nodes.size() );
    
    for ( unsigned int i = 0; i < nodes.size(); i++ )
        nodes[i].add_coords( center_mass );


        
    float r = 0.1;
    float k = 0.5f * r;
    unsigned int number_of_nodes = nodes.size();
    
    // O(N^2) alarm
    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        for ( unsigned int j = i+1; j < number_of_nodes; j++ ) {
            xyz delta_xyz = nodes[j].getCoords() - nodes[i].getCoords();
            xyz delta_velocity = delta_xyz * ( r / len_squared(delta_xyz) );
            nodes[j].add_velocity( delta_velocity );
            nodes[i].add_velocity( delta_velocity * (-1) );
        }
    
    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        for ( Node* neighbour : nodes[i].getEdges() ) {
            xyz delta_xyz = neighbour->getCoords() - nodes[i].getCoords();
            xyz delta_velocity = delta_xyz * ( -k * std::sqrt( len_squared(delta_xyz) ) );
            neighbour->add_velocity( delta_velocity );
            nodes[i].add_velocity( delta_velocity * (-1) );
        }
    // O(N^2) alarm end

    Node::max_velocity = 0;

    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        nodes[i].update_coords();

    std::cout << Node::max_velocity << std::endl;
};

void Graph::display(sf::RenderWindow& window) {

    // numbers describing to the graph as a whole
    
    if ( yaw > 6.28f ) yaw -= 6.28f;
    if ( yaw < -6.28f ) yaw += 6.28f;

    if ( pitch > 1.57f ) pitch = 1.57f;
    if ( pitch < -1.57f ) pitch = -1.57f;
    
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

    // POV value
    POV = {0, 0, 0};
    POV += (small_corner * 0.5f);
    POV += (big_corner * 0.5f);

    float maxwidth = big_corner.x - small_corner.x;
    if ( big_corner.y - small_corner.y > maxwidth ) maxwidth = big_corner.y - small_corner.y;
    if ( big_corner.z - small_corner.z > maxwidth ) maxwidth = big_corner.z - small_corner.z;
    float scale = 1.5f * window_center.y / maxwidth;

    closest_farthest_z = {0, 0};
    for ( int i = 0; i < 8; i++ ) {
        xyz corner_window_coords = calc_window_coords(
            { (i % 2) * small_corner.x + !(i % 2) * big_corner.x,
            (i/2 % 2) * small_corner.y + !(i/2 % 2) * big_corner.y,
            (i/4 % 8) * small_corner.z + !(i/4 % 2) * big_corner.z },
            scale );
            
        if ( corner_window_coords.z > closest_farthest_z.x )
            closest_farthest_z.x = corner_window_coords.z;
        if ( corner_window_coords.z < closest_farthest_z.y )
            closest_farthest_z.y = corner_window_coords.z;
    }

    display_grid( window, scale );



    // numbers describing each node

    std::map<Node*, xyz> nodes_window_coords;

    for ( unsigned int i = 0; i < number_of_nodes; i++ )
        nodes_window_coords[ &nodes[i] ] =
            calc_window_coords( nodes[i].getCoords(), scale );

    for ( unsigned int i = 0; i < number_of_nodes; i++ ) {

        display_node( window, window_center, nodes_window_coords[ &nodes[i] ] );

        std::vector<Node*> neighbours = nodes[i].getEdges();
        for ( unsigned int j = 0; j < neighbours.size(); j++ )
            display_edge( window, window_center, nodes_window_coords[ &nodes[i] ], nodes_window_coords[ neighbours[j] ] );
    }

};

void Graph::display_point(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords, float RadiusInPixels, sf::Color color) {
    
    // perspective
    coords.x *= perspective_multiplier(coords.z);
    coords.y *= perspective_multiplier(coords.z);

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);
    circle.setPosition( sf::Vector2f({coords.x, coords.y}) + window_center );
    window.draw(circle);
};

void Graph::display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color color) {
    display_line( window, window_center, c1, c2, color, color );
};

void Graph::display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color col1, sf::Color col2) {

    // perspective
    c1.x *= perspective_multiplier(c1.z);
    c1.y *= perspective_multiplier(c1.z);
    c2.x *= perspective_multiplier(c2.z);
    c2.y *= perspective_multiplier(c2.z);

    sf::Vertex line[] = {
        sf::Vertex( sf::Vector2f({c1.x, c1.y}) + window_center ),
        sf::Vertex( sf::Vector2f({c2.x, c2.y}) + window_center )
    };
    line[0].color = col1;
    line[1].color = col2;
    window.draw(line, 2, sf::Lines);
};

xyz Graph::calc_window_coords(xyz coords, float scale) {

    coords -= POV;

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

    final_coords.x = final_coords.x * scale;
    final_coords.y = final_coords.y * scale;
    final_coords.z = final_coords.z * scale;

    return final_coords;
};

float Graph::perspective_multiplier(float z) {
    float k = 2000.f;
    return ( k / ( closest_farthest_z.x + k - z ) );
};

void Graph::display_node(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords) {
    display_point( window, window_center, coords, 4, sf::Color::White );
};

void Graph::display_edge(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2) {
    display_line( window, window_center, c1, c2, sf::Color::White );
};

void Graph::display_grid(sf::RenderWindow& window, float scale) {

    sf::Color grid_color = sf::Color::Blue;
    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };

    // small_corner -= {0.5f, 0.5f, 0.5f};
    // big_corner += {0.5f, 0.5f, 0.5f};

    std::map<int, xyz> corners;

    // big brain bool logic
    for ( int i = 0; i < 8; i++ ) {
        corners[i] = calc_window_coords(
            { (i % 2) * small_corner.x + !(i % 2) * big_corner.x,
            (i/2 % 2) * small_corner.y + !(i/2 % 2) * big_corner.y,
            (i/4 % 8) * small_corner.z + !(i/4 % 2) * big_corner.z },
            scale );
    }

    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[i], corners[i+4], grid_color );
    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[2*i], corners[2*i+1], grid_color );
    for ( int i = 0; i < 2; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color );
    for ( int i = 4; i < 6; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color );

    xyz center_window_coords = calc_window_coords( POV, scale );
    display_point( window, window_center, center_window_coords, 4, grid_color );
};

void Node::update_coords() {

    float vel_limit = 10;

    if ( len_squared(velocity) > vel_limit*vel_limit ) {
        float k = vel_limit / std::sqrt( len_squared(velocity) );
        velocity.x *= k; velocity.y*= k; velocity.z*= k;
    }

    if ( max_velocity < std::sqrt( len_squared(velocity) ) )
        max_velocity = std::sqrt( len_squared(velocity) );

    coords += velocity;
    velocity *= 0.60f;
    
};