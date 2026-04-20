#ifndef _PHYSGRAPH_TOOLS_
#define _PHYSGRAPH_TOOLS_

#include <SFML/Graphics.hpp>

struct xyz
{
    float x;
    float y;
    float z;

    void print();

    xyz operator+(const xyz& other) const {
        return {x + other.x, y + other.y, z + other.z};
    };

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

unsigned int rnd_number(unsigned a, unsigned b);

xyz rnd_xyz_direction(float amplitude);

float len_squared(xyz delta);

sf::Color depth_shading(float depth_0to1, const sf::Color& color);

#endif