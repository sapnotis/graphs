#ifndef _GRAPH_PHYSICS_
#define _GRAPH_PHYSICS_

#include "graph-components.hpp"

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

class PhNode : public Node
{
private:
    static xyz projection_line;
    xyz coords;
    xyz velocity;
public:
    PhNode();
    ~PhNode();

    xyz calculate_projection();
};

#endif