#ifndef _GRAPH_COMPONENTS_
#define _GRAPH_COMPONENTS_

#include "tools.hpp"
#include <vector>
#include <list>

#include <iostream>
#include <SFML/Graphics.hpp>

class Graph;
class Node;

class Graph
{
private:
    const bool allow_equal_nodes; // meant to stay false (one node represents one state of values)
    const bool allow_multiple_edges; // meant to stay false (different moves of the same result are impossible)
    std::list<Node> nodes;

    xyz POV;
    const float POV_multiplier;
    Node* selected_node;
    int selected_neighbour;

    float yaw;
    float pitch;
    xyz small_corner;
    xyz big_corner;
public:
    Graph();
    ~Graph();

    Node* emplace_node(std::vector<int> values);

    void emplace_edge(std::vector<int> val_f, std::vector<int> val_s);
    void emplace_edge(Node* f, std::vector<int> val_s);
    void emplace_edge(std::vector<int> val_f, Node* s);
    void emplace_edge(Node* f, Node* s);

    void erase_node(std::vector<int> values);
    void erase_edge(std::vector<int> val_f, std::vector<int> val_s);
    void erase_edge(Node* f, Node* s);

    Node* findNode(std::vector<int> values);
    std::vector<Node*> getNodes();
    void rollcall();

    // V3 and SFML

    Node* get_selected_node() { return selected_node; };
    void set_selected_node(Node* sel_node) { selected_node = sel_node; };
    int get_selected_neighbour() { return selected_neighbour; };
    void set_selected_neighbour(int sel_tar) { selected_neighbour = sel_tar; };

    void tick();
    
    void display(sf::RenderWindow& window);

    void display_point(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords, float RadiusInPixels, sf::Color color);
    void display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color col1, sf::Color col2);

    void addYaw(float dyaw) { yaw += dyaw; };
    void addPitch(float dpitch) { pitch += dpitch; };
    void resetYawPitch() { yaw = pitch = 0; };

    xyz calc_window_coords(xyz coords, float scale);
    float perspective_multiplier(float z);
    void display_grid(sf::RenderWindow& window, sf::Color grid_color);
    void display_xyz_axes(sf::RenderWindow& window, float scale);
};

class Node
{
private:
    std::vector<int> values;
    std::vector<Node*> edges;
    xyz coords;
    xyz velocity;
    sf::Color color;
public:
    bool checked;

    inline const static float velocity_limit = 12;
    inline const static float interact_koef = 0.04f;
    inline const static float vel_multiplier = 0.66f;
    
    // Consider replacing std::vector with std::array
    // cause it's literally array of numbers
    // no vector logic is needed
    Node(std::vector<int> values);
    Node(std::vector<int> values, xyz coords);
    ~Node();

    bool operator==(const Node& other) const { return ( values == other.values ); }

    void add_edge(Node* node);
    bool find_edge(Node* node);
    void forget_edge(Node* node);

    std::vector<int> getValues() { return values; };
    std::vector<Node*> getEdges() { return edges; }; 

    // V3 and SFML
    
    void set_coords(xyz crd) { coords = crd; };
    void add_coords(xyz dcrd) { coords += dcrd; };
    xyz getCoords() const { return coords; };

    void set_velocity(xyz vel) { velocity = vel; };
    void add_velocity(xyz dvel) { velocity += dvel; };
    xyz getVelocity() const { return velocity; };

    void set_color(sf::Color col) { color = col; };
    sf::Color getColor() const { return color; }

    void update_coords();
};

#endif