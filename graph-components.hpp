#ifndef _GRAPH_COMPONENTS_
#define _GRAPH_COMPONENTS_

#include "tools.hpp"
#include <deque>
#include <vector>
#include <map>

#include <iostream>
#include <SFML/Graphics.hpp>

class Graph;
class Node;

class Graph
{
private:
    const bool allow_equal_nodes; // meant to stay false (one node represents one state of values)
    const bool allow_multiple_edges; // meant to stay false (different moves of the same result are impossible)
    std::deque<Node> nodes;

    xyz POV;
    xyz small_corner;
    xyz big_corner;
    float yaw;
    float pitch;
    sf::Vector2f closest_farthest_z;
public:
    Graph();
    ~Graph();
    bool is_corrupted() const;

    Node* emplace_node(std::vector<int> values);

    void emplace_edge(std::vector<int> val_f, std::vector<int> val_s);
    void emplace_edge(Node* f, std::vector<int> val_s);
    void emplace_edge(std::vector<int> val_f, Node* s);
    void emplace_edge(Node* f, Node* s);

    void erase_node(Node* node);
    void erase_node(const Node& node);

    void erase_edge(std::vector<int> val_f, std::vector<int> val_s);
    void erase_edge(Node* f, Node* s);

    Node* findNode(std::vector<int> values);
    std::vector<Node*> getNodes();
    void rollcall();

    // V3 and SFML

    void update_nodes();
    
    void display(sf::RenderWindow& window);

    void display_point(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords, float RadiusInPixels, sf::Color color);
    void display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color color);
    void display_line(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2, sf::Color col1, sf::Color col2);

    void addYaw(float dyaw) { yaw += dyaw; };
    void addPitch(float dpitch) { pitch += dpitch; };
    void resetYawPitch() { yaw = pitch = 0; };

    xyz calc_window_coords(xyz coords, float scale);
    float perspective_multiplier(float z);
    void display_node(sf::RenderWindow& window, sf::Vector2f window_center, xyz coords);
    void display_edge(sf::RenderWindow& window, sf::Vector2f window_center, xyz c1, xyz c2);
    void display_grid(sf::RenderWindow& window, float scale);
};

class Node
{
private:
    std::vector<int> values;
    std::vector<Node*> edges;
    xyz coords;
    xyz velocity;
public:
    bool checked;

    inline static float max_velocity = 0; // debug
    
    Node(std::vector<int> values);
    Node(std::vector<int> values, xyz coords);
    ~Node();

    bool operator==(const Node& other) const { return ( values == other.values ); }

    void add_edge(Node* node);
    bool find_edge(Node* node);
    void forget_edge(Node* node);

    std::vector<int> getValues() { return values; };
    std::vector<Node*> getEdges() { return edges; };
    xyz getCoords() const { return coords; };
    xyz getVelocity() const { return velocity; };

    // V3 and SFML

    void set_velocity(xyz vel) { velocity = vel; };
    void set_coords(xyz crd) { coords = crd; };
    void add_velocity(xyz dvel) { velocity += dvel; };
    void add_coords(xyz dcrd) { coords += dcrd; };

    void update_coords();
};

#endif