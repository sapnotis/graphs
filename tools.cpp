#include "tools.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include <random>
// #include <chrono>

void xyz::display() {
    std::cout << x << " " << y << " " << z << std::endl;
}

void xy::display() {
    std::cout << x << " " << y << std::endl;
}

xyz xyz_rnd_direction(float amplitude) {

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

unsigned int brightness(float depth) {
    return (unsigned)(55*depth) + 200;
};