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

    void update_nodes();
    
    xyz calc_window_coords(xyz coords, float scale, xy window_center);
    void display_node(sf::RenderWindow& window, float scale, xyz coords);
    void display_edge(sf::RenderWindow& window, xyz p1, xyz p2);

    void display_point(sf::RenderWindow& window, float scale, xyz point, sf::Color color);
    void display_point(sf::RenderWindow& window, float scale, xy point, sf::Color color);

    void display_line(sf::RenderWindow& window, xy p1, xy p2, sf::Color color);
    void display_line(sf::RenderWindow& window, xy p1, xy p2, sf::Color col1, sf::Color col2);

    void display_grid(sf::RenderWindow& window, float scale);
    void display(sf::RenderWindow& window, float scale);

    void addYaw(float dyaw) {
        yaw += dyaw;
        if ( yaw > 6.28f )
            yaw -= 6.28f;
        if ( yaw < -6.28f )
            yaw += 6.28f;
    };
    void addPitch(float dpitch) {
        pitch += dpitch;
        if ( pitch > 1.2f ) pitch -= dpitch;
        if ( pitch < -1.2f ) pitch -= dpitch;
    };
    void zeroYawPitch() { yaw = pitch = 0; };
};

class Node
{
private:
    std::vector<int> values;
    std::vector<Node*> edges;
    xyz coords;
public:
    bool checked;
    
    Node(std::vector<int> values);
    Node(std::vector<int> values, xyz coords);
    ~Node();

    bool operator==(const Node& other) const { return ( values == other.values ); }

    void add_edge(Node* node);
    bool find_edge(Node* node);
    void forget_edge(Node* node);

    void update_coords();

    std::vector<int> getValues() { return values; };
    std::vector<Node*> getEdges() { return edges; };
    xyz getCoords() const { return coords; };
};

#endif