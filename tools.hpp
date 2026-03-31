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
// xy window_xy(xy coords, sf::RenderWindow& window, float scale);
xy window_xy(xy coords, float scale);

#endif