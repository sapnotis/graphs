#include "graph-components.hpp"

#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>

Graph::Graph() : allow_equal_nodes(false), allow_multiple_edges(false),
    POV({0, 0, 0}), POV_multiplier(0.8f), selected_node(nullptr), yaw(0), pitch(0),
    small_corner({0, 0, 0}), big_corner({0, 0, 0}), closest_farthest_z({0, 0}) { };

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

void Graph::update_nodes() {

    xyz center_mass = {0, 0, 0};
    for ( Node node : nodes )
        center_mass += node.getCoords();
    
    center_mass *= ( -1.f / nodes.size() );
    
    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        node->add_coords( center_mass );


        
    float r = Node::interact_koef;
    float k = 0.25f * r;
    
    // O(N^2) alarm
    for ( auto first = nodes.begin(); first != nodes.end(); first++ )
        for ( auto second = std::next(first, 1); second != nodes.end(); second++ ) {
            xyz delta_xyz = second->getCoords() - first->getCoords();            
            float delta_len_squared = len_squared(delta_xyz);

            // not physically meaningfull, honestly
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
    // O(N^2) alarm end

    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        node->update_coords();
};

void Graph::display(sf::RenderWindow& window) {

    // numbers describing to the graph as a whole
    
    if ( yaw > 6.28f ) yaw -= 6.28f;
    if ( yaw < -6.28f ) yaw += 6.28f;

    if ( pitch > 1.57f ) pitch = 1.57f;
    if ( pitch < -1.57f ) pitch = -1.57f;
    
    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };
    
    // outline box
    
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

    small_corner += {-1, -1, -1};
    big_corner += {1, 1, 1};

    // POV

    xyz POV_goal = (small_corner + big_corner) * 0.5f;
    if ( selected_node )
        POV_goal = selected_node->getCoords();

    POV = POV * POV_multiplier + POV_goal * ( 1 - POV_multiplier );

    float scale = 100;
    if ( ! selected_node ) {
        float maxwidth = big_corner.x - small_corner.x;
        if ( big_corner.y - small_corner.y > maxwidth ) maxwidth = big_corner.y - small_corner.y;
        if ( big_corner.z - small_corner.z > maxwidth ) maxwidth = big_corner.z - small_corner.z;
        scale = 1.75f * window_center.y / maxwidth;
    }

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

    for ( auto node = nodes.begin(); node != nodes.end(); node++ )
        nodes_window_coords[ &(*node) ] =
            calc_window_coords( node->getCoords(), scale );

    for ( auto node = nodes.begin(); node != nodes.end(); node++ ) {

        display_point( window, window_center, nodes_window_coords[ &(*node) ], 4, node->getColor() );

        std::vector<Node*> neighbours = node->getEdges();
        for ( Node* neighbour : neighbours )
            display_line( window, window_center, nodes_window_coords[ &(*node) ], nodes_window_coords[ neighbour ], node->getColor(), neighbour->getColor() );
    }
};

void Graph::display_point(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords, float RadiusInPixels, sf::Color color) {
    
    // perspective
    coords.x *= perspective_multiplier(coords.z);
    coords.y *= perspective_multiplier(coords.z);

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);
    circle.setPosition( window_center + sf::Vector2f({coords.x, -coords.y}) );
    window.draw(circle);
};

void Graph::display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color col1, sf::Color col2) {

    // perspective
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
    float k = 2000.f;
    return ( k / ( closest_farthest_z.x + k - z ) );
};

void Graph::display_grid(sf::RenderWindow& window, float scale) {

    sf::Color grid_color = sf::Color(172, 172, 172);
    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };

    // circumscribed parallelogram (is this even a word? circumscribed)
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
        display_line( window, window_center, corners[i], corners[i+4], grid_color, grid_color );
    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[2*i], corners[2*i+1], grid_color, grid_color );
    for ( int i = 0; i < 2; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color, grid_color );
    for ( int i = 4; i < 6; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color, grid_color );

    // Window center & xyz trihedron
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
        color = sf::Color::Red;
    }
    else {
        color = sf::Color::White;
    }

    coords += velocity;
    velocity *= vel_multiplier;
};