#ifndef _PHYSGRAPH_TOOLS_
#define _PHYSGRAPH_TOOLS_

#include <SFML/Graphics.hpp>

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

xyz xyz_rnd_direction(float amplitude);

unsigned int brightness(float depth);

#endif