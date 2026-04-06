#ifndef _PHYSGRAPH_TOOLS_
#define _PHYSGRAPH_TOOLS_

#include <SFML/Graphics.hpp>

struct xyz
{
    float x;
    float y;
    float z;

    void print();

    xyz operator-(const xyz& other) const {
        return {x - other.x, y - other.y, z - other.z};
    };

    xyz& operator+=(const xyz& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    };

    xyz& operator-=(const xyz& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    };

    xyz& operator*=(const float scale) {
        x *= scale; y *= scale; z *= scale;
        return *this;
    };

    xyz operator*(const float scale) const {
        return {x*scale, y*scale, z*scale};
    };
};

xyz xyz_rnd_direction(float amplitude);

float len_squared(xyz delta);

// sf::Color color_of_depth(float depth);

#endif