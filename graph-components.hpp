#ifndef _GRAPH_COMPONENTS_
#define _GRAPH_COMPONENTS_

#include "tools.hpp"
#include <deque>
#include <vector>

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
public:
    xyz center;
    float yaw;
    float pitch;

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

    void display_node(sf::RenderWindow& window, float scale, xyz point);
    void display_edge(sf::RenderWindow& window, float scale, xyz p1, xyz p2);

    xyz calc_node_screen_coords(const Node& node);
    void display(sf::RenderWindow& window, float scale);

    xy getYawPitch() { return {yaw, pitch}; };
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

    // void display_self(sf::RenderWindow& window, float scale);
    // void display_edges(sf::RenderWindow& window, float scale);

    std::vector<int> getValues() { return values; };
    std::vector<Node*> getEdges() { return edges; };
    xyz getCoords() const { return coords; };
};

#endif