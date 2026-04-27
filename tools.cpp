#include "tools.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include <random>
#include <cmath>

void xyz::print() {
    std::cout << x << " " << y << " " << z << std::endl;
}

unsigned int rnd_number(unsigned a, unsigned b) {
    if (a > b) {
        unsigned tmp = a;
        a = b;
        b = tmp;
    }

    std::random_device rd; 
    std::mt19937 engine( rd() ); 
    std::uniform_int_distribution<int> dist(a, b);

    return dist(engine);
};

xyz rnd_xyz_direction(float amplitude) {

    if ( !amplitude )
        amplitude = 1;

    std::random_device rd; 
    std::mt19937 engine( rd() ); 
    std::uniform_int_distribution<int> dist(-1000, 1000);

    // I'm sorry (not spherically symmetrical)
    xyz dir;
    dir = {
        0.001f * amplitude * dist(engine),
        0.001f * amplitude * dist(engine),
        0.001f * amplitude * dist(engine)
    };

    // I'm so sorry
    if ( !dir.x && !dir.y && !dir.z )
        dir.x = amplitude;

    return dir;
}

float len_squared(xyz delta) {
    return (
        delta.x * delta.x
        + delta.y * delta.y
        + delta.z * delta.z
    );
};

sf::Color depth_shading(float depth_0to1, const sf::Color& color) {
    sf::Color shaded_color = color;
    shaded_color.r *= 1 - 0.9*depth_0to1;
    shaded_color.g *= 1 - 0.9*depth_0to1;
    shaded_color.b *= 1 - 0.9*depth_0to1;
    return shaded_color;
};