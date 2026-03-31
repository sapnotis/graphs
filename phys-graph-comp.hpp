#ifndef _GRAPH_PHYSICS_
#define _GRAPH_PHYSICS_

#include "graph-components.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

struct xyz
{
    float x;
    float y;
    float z;

    void display();
};

struct xy
{
    float x;
    float y;

    void display();
};

xyz xyz_rnd_direction();
xy window_xy(xy coords, sf::RenderWindow& window, float scale);

class PhysSpace;
class PhysNode;

class PhysSpace
{
private:
    Graph* graph;
    std::vector<PhysNode> nodes;
public:
    PhysSpace(Graph& graph);
    ~PhysSpace();

    void sync_nodes();
    
    void update_nodes();
    void update_observer();
    void update_observer_goal();

    void display_all(sf::RenderWindow& window, float scale);
};

class PhysNode
{
private:
    // std::vector<PhysNode*> neighbours;
    static xyz observer;
    static xyz observer_goal;
    static xyz projection_line;
public:
    xyz coords;
    xy projection;

    PhysNode(Node* node, xyz coords);
    PhysNode(Node* node);
    ~PhysNode();

    void update_coords();
    void update_projection();

    void display_self(sf::RenderWindow& window, float scale);
    void display_edges(sf::RenderWindow& window, float scale);
};

#endif