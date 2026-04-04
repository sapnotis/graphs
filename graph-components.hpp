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

    xyz center;
    xyz small_corner;
    xyz big_corner;
    float yaw;
    float pitch;
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
    
    void display(sf::RenderWindow& window, float scale);

    void display_point(sf::RenderWindow& window, sf::Vector2f point, float RadiusInPixels, sf::Color color);
    void display_line(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Color color);
    void display_line(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Color col1, sf::Color col2);

    void addYaw(float dyaw) { yaw += dyaw; };
    void addPitch(float dpitch) { pitch += dpitch; };
    void resetYawPitch() { yaw = pitch = 0; };

    xyz calc_window_coords(xyz coords, float scale, sf::Vector2f window_center);
    void display_node(sf::RenderWindow& window, xyz coords);
    void display_edge(sf::RenderWindow& window, xyz c1, xyz c2);
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

    // V3 and SFML

    void update_velocity();
    void update_coords();
};

#endif