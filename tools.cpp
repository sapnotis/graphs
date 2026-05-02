#include "tools.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include <random>
#include <cmath>

void xyz::print() {
    std::cout << x << " " << y << " " << z << std::endl;
}

xyz rnd_xyz_direction(float amplitude) {

    if ( !amplitude )
        amplitude = 1;

    std::random_device rd; 
    std::mt19937 engine( rd() ); 
    std::uniform_int_distribution<int> dist(-1000, 1000);

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
    shaded_color.r *= 1 - 0.75*depth_0to1;
    shaded_color.g *= 1 - 0.75*depth_0to1;
    shaded_color.b *= 1 - 0.75*depth_0to1;
    return shaded_color;
};