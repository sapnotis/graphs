#ifndef _GRAPH_PHYSICS_
#define _GRAPH_PHYSICS_

#include "graph-components.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

struct xyz
{
    double x;
    double y;
    double z;

    void display();

    // xyz& operator=(const xyz& other) {
    //     x = other.x;
    //     y = other.y;
    //     z = other.z;
    //     return *this;
    // }

    // xyz& operator+(const xyz& other) {
    //     x += other.x;
    //     y += other.y;
    //     z += other.z;
    //     return *this;
    // }
};

xyz rnd_direction();

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
    void update_all();
    void display_all();
};

class PhysNode
{
private:
    Node* node;
    static xyz projection_line;
public:
    xyz coords;
    xyz velocity;
    PhysNode(Node* node, xyz coords);
    PhysNode(Node* node);
    ~PhysNode();

    void update_velocity();
    void update_coord();
    xyz calculate_projection();

    Node* getNode() { return node; };
};

#endif