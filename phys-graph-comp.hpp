#ifndef _GRAPH_PHYSICS_
#define _GRAPH_PHYSICS_

#include "graph-components.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

// things should have been inherited from Graph and Node,
// but concept changed once again, i'm sorry

struct xyz
{
    double x;
    double y;
    double z;

    /*xyz operator+(const xyz& other) const { 
        xyz tmp = {
            x + other.x,
            y + other.y,
            z + other.z
        };
        return tmp;
    }*/
};

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
    xyz coords;
    xyz velocity;
    static xyz projection_line;
public:
    PhysNode(Node* node, xyz coords);
    PhysNode(Node* node);
    ~PhysNode();

    void update_velocity();
    void update_coord();
    xyz calculate_projection();

    Node* getNode() { return node; };
    xyz getCoords() { return coords; };
    xyz getVelocity() { return velocity; };
};

#endif