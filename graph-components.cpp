#include "graph-components.hpp"

#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>

Graph::Graph() : allow_equal_nodes(false), allow_multiple_edges(false),
    POV({0, 0, 0}), POV_multiplier(0.5f), selected_node(nullptr), selected_neighbour(-1),
    yaw(0), pitch(0), small_corner({0, 0, 0}), big_corner({0, 0, 0}) { };

Graph::~Graph() { };

Node* Graph::emplace_node(std::vector<int> values) {
    if ( allow_equal_nodes ) {
        nodes.emplace_front(values);
        return &( nodes.back() );
    }
    
    if ( findNode(values) ) {
        std::cout << "(!) Graph attempted to double node" << std::endl;
        return nullptr;
    }
    
    return &( nodes.emplace_front(values) );
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

    if ( ! allow_multiple_edges )
        if ( f->find_edge(s) || s->find_edge(f) ) {
            std::cout << "(!) Graph attempted to double edge" << std::endl;
            return;
        }
    
    f->add_edge( s );
    s->add_edge( f );
}

void Graph::erase_node(std::vector<int> values) {
    Node ref(values);
    
    auto it = std::find(nodes.begin(), nodes.end(), ref);
    if ( it == nodes.end() ) {
        std::cout << "(!) Graph couldn't erase node" << std::endl;
        return;
    }
    
    for ( Node* neighbour : it->getEdges() )
        neighbour->forget_edge( &(*it) ); // it so cursed i'm so sorry
    nodes.erase(it);
};

void Graph::erase_edge(std::vector<int> val_f, std::vector<int> val_s) {
    Node* f = findNode(val_f);
    Node* s = findNode(val_s);
    erase_edge(f, s);
};

void Graph::erase_edge(Node* f, Node* s) {
    if ( ! f->find_edge( s ) || ! s->find_edge( f ) ) {
        std::cout << "(!) Graph couldn't erase edge" << std::endl;
        return;
    }

    f->forget_edge( s );
    s->forget_edge( f );
};

Node* Graph::findNode(std::vector<int> values) {
    Node ref(values);
    
    auto it = std::find(nodes.begin(), nodes.end(), ref);
    if ( it == nodes.end() )
        return nullptr;
    return ( &(*it) );
}

std::vector<Node*> Graph::getNodes() {
    std::vector<Node*> node_ptrs;

    for ( auto it = nodes.begin(); it != nodes.end(); it++ )
        node_ptrs.push_back( &(*it) );

    return node_ptrs;
};

void Graph::rollcall() {
    std::cout << std::endl;
    std::cout << "=Nodes rollcall=" << std::endl;
    for ( auto it = nodes.begin(); it != nodes.end(); it++ ) {
        std::cout << &(*it);
        std::cout << "\t";
        for ( int num : it->getValues() )
            std::cout << " " << num;
        std::cout << "\t";
        for ( Node* neighbour : it->getEdges() )
            std::cout << " " << neighbour;
        std::cout << std::endl;
    }
    std::cout << "=Rollcall ended=" << std::endl;
    std::cout << std::endl;
}

Node::Node(std::vector<int> values)
    : values(values), coords({0, 0, 0}), velocity({0, 0, 0}), color(sf::Color::White), checked(false) { };

Node::Node(std::vector<int> values, xyz coords)
    : values(values), coords(coords), velocity({0, 0, 0}), color(sf::Color::White), checked(false) { };

Node::~Node() { };

void Node::add_edge(Node* node) {
    edges.push_back(node);
};

bool Node::find_edge(Node* node) {
    
    auto it = std::find(edges.begin(), edges.end(), node);
    if ( it == edges.end() )
        return false;
    return true;
};

void Node::forget_edge(Node* node) {

    auto it = std::find(edges.begin(), edges.end(), node);
    if ( it == edges.end() ) {
        std::cout << "(!) Node couldn't forget edge" << std::endl;
        return;
    }
    edges.erase(it);
};

// V3 and SFML

void Graph::tick() {

    xyz center_mass = {0, 0, 0};
    for ( Node node : nodes )
        center_mass += node.getCoords();
    
    center_mass *= ( -1.f / nodes.size() );
    
    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        node->add_coords( center_mass );

    float r = Node::interact_koef;
    float k = 0.25f * r;
    
    // O(NN) alarm
    for ( auto first = nodes.begin(); first != nodes.end(); first++ )
        for ( auto second = std::next(first, 1); second != nodes.end(); second++ ) {
            xyz delta_xyz = second->getCoords() - first->getCoords();            
            float delta_len_squared = len_squared(delta_xyz);
            
            if ( delta_len_squared <= 0.1f )
                delta_len_squared = 0.1f;

            xyz delta_velocity = delta_xyz * ( r / delta_len_squared );
            first->add_velocity( delta_velocity * (-1) );
            second->add_velocity( delta_velocity );
        }
    
    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        for ( Node* neighbour : node->getEdges() ) {
            xyz delta_xyz = neighbour->getCoords() - node->getCoords();
            float delta_len_squared = len_squared(delta_xyz);
            
            xyz delta_velocity = delta_xyz * ( -k * std::sqrt(delta_len_squared) );
            node->add_velocity( delta_velocity * (-1) );
            neighbour->add_velocity( delta_velocity );
        }
    // O(NN) alarm end

    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        node->update_coords();
};

void Graph::display(sf::RenderWindow& window) {

    sf::Color selection_color = sf::Color(255, 165, 0);
    
    if ( yaw > 6.28f ) yaw -= 6.28f;
    if ( yaw < -6.28f ) yaw += 6.28f;

    if ( pitch > 1.57f ) pitch = 1.57f;
    if ( pitch < -1.57f ) pitch = -1.57f;
    
    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };

    xyz POV_goal = {0, 0, 0};
    if ( selected_node )
        POV_goal = selected_node->getCoords();

    POV = POV * POV_multiplier + POV_goal * ( 1 - POV_multiplier );



    std::map<Node*, xyz> nodes_window_coords;

    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        nodes_window_coords[ &(*node) ] = calc_window_coords( node->getCoords(), 1 );
    
    nodes.sort( [nodes_window_coords](Node& a, Node& b) {
        return nodes_window_coords.at(&a).z < nodes_window_coords.at(&b).z;
    } );

    small_corner = big_corner = {0, 0, 0};

    small_corner.z = nodes_window_coords[ &(nodes.back()) ].z;
    big_corner.z = nodes_window_coords[ &(nodes.front()) ].z;

    for ( auto& [n, coords] : nodes_window_coords ) {
        if ( coords.x > big_corner.x )
            big_corner.x = coords.x;
        if ( coords.x < small_corner.x )
            small_corner.x = coords.x;
        if ( coords.y > big_corner.y )
            big_corner.y = coords.y;
        if ( coords.y < small_corner.y )
            small_corner.y = coords.y;
    }

    float scale = 30;
    if ( ! selected_node ) {
        float maxdist_sqr = 0;
        for ( Node node : nodes ) {
            float len_sqr = len_squared( node.getCoords() );
            if ( maxdist_sqr < len_sqr )
                maxdist_sqr = len_sqr;
            }

        if ( window_center.y < window_center.x )
            scale = window_center.y / std::sqrt(maxdist_sqr);
        else
            scale = window_center.x / std::sqrt(maxdist_sqr);
    }

    for ( auto& [n, coords] : nodes_window_coords )
        coords *= scale;
    small_corner *= scale;
    big_corner *= scale;

    // display

    display_xyz_axes(window, scale);

    for ( auto node = nodes.begin(); node != nodes.end(); node++ ) {
        sf::Color color = depth_shading(
            (small_corner.z - nodes_window_coords.at( &(*node) ).z)
            / (small_corner.z - big_corner.z),
            node->getColor()
        );
        if ( selected_node )
            color.a = 55;
        node->set_color( color );
    }

    for ( auto node = nodes.begin(); node != nodes.end(); node++ ) {

        display_point( window, window_center, nodes_window_coords[ &(*node) ], 4, node->getColor() );
        std::vector<Node*> neighbours = node->getEdges();

        for ( Node* neighbour : neighbours )
            display_line( window, window_center, nodes_window_coords[ &(*node) ], nodes_window_coords[ neighbour ], node->getColor(), neighbour->getColor() );

    }

    if ( selected_node ) {
        display_point( window, window_center, nodes_window_coords[ selected_node ], 4, selection_color );
        if ( selected_neighbour != -1 ) {
            display_line( window, window_center, nodes_window_coords[ selected_node ], nodes_window_coords[ selected_node->getEdges()[selected_neighbour] ], selection_color, selection_color );
        }
    }
};

void Graph::display_point(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords, float RadiusInPixels, sf::Color color) {
    
    coords.x *= perspective_multiplier(coords.z);
    coords.y *= perspective_multiplier(coords.z);

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);
    circle.setPosition( window_center + sf::Vector2f({coords.x, -coords.y}) );
    window.draw(circle);
};

void Graph::display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color col1, sf::Color col2) {

    c1.x *= perspective_multiplier(c1.z);
    c1.y *= perspective_multiplier(c1.z);
    c2.x *= perspective_multiplier(c2.z);
    c2.y *= perspective_multiplier(c2.z);

    sf::Vertex line[] = {
        sf::Vertex( window_center + sf::Vector2f({c1.x, -c1.y}) ),
        sf::Vertex( window_center + sf::Vector2f({c2.x, -c2.y}) )
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
    float k = 1200.f;
    return ( k / ( k - z ) );
};

void Graph::display_grid(sf::RenderWindow& window, sf::Color grid_color) {

    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };

    // circumscribed parallelogram (is this even a word? circumscribed)
    std::map<int, xyz> corners;

    // big brain bool logic
    for ( int i = 0; i < 8; i++ ) {
        corners[i] =
            { (i % 2) * small_corner.x + !(i % 2) * big_corner.x,
            (i/2 % 2) * small_corner.y + !(i/2 % 2) * big_corner.y,
            (i/4 % 8) * small_corner.z + !(i/4 % 2) * big_corner.z };
    }

    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[i], corners[i+4], grid_color, grid_color );
    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[2*i], corners[2*i+1], grid_color, grid_color );
    for ( int i = 0; i < 2; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color, grid_color );
    for ( int i = 4; i < 6; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color, grid_color );
};

void Graph::display_xyz_axes(sf::RenderWindow& window, float scale) {

    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };
    
    std::map<int, xyz> others;

    others[0] = calc_window_coords( POV, scale );
    float dir_len = 100 / scale;
    others[1] = calc_window_coords( {POV.x + dir_len, POV.y, POV.z}, scale );
    others[2] = calc_window_coords( {POV.x, POV.y + dir_len, POV.z}, scale );
    others[3] = calc_window_coords( {POV.x, POV.y, POV.z + dir_len}, scale );

    display_line( window, window_center, others[0], others[3], sf::Color::Blue, sf::Color::Blue );
    display_line( window, window_center, others[0], others[2], sf::Color::Green, sf::Color::Green );
    display_line( window, window_center, others[0], others[1], sf::Color::Red, sf::Color::Red );
};

void Node::update_coords() {

    if ( len_squared(velocity) > velocity_limit*velocity_limit ) {
        float k = velocity_limit / std::sqrt( len_squared(velocity) );
        velocity.x *= k; velocity.y*= k; velocity.z*= k;
        color = sf::Color(255, 0, 0);
    }
    else
        color = sf::Color::White;

    coords += velocity;
    velocity *= vel_multiplier;
};