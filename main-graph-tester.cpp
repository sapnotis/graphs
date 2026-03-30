#include <iostream>
#include "graph-components.hpp"
#include "phys-graph-comp.hpp"
#include <SFML/Graphics.hpp>

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    g.emplace_node({1, 1});
    g.emplace_edge( g.findNode({1, 1}), g.emplace_node({1, 2}) );
    g.emplace_edge( g.findNode({1, 2}), g.emplace_node({1, 3}) );
    g.emplace_edge( g.findNode({1, 1}), g.emplace_node({2, 1}) );

    g.rollcall();

    std::cout << "erase_node getNodes().front()" << std::endl; 
    g.erase_node( *( g.getNodes().front() ) );

    g.rollcall();

    std::cout << "erase_node getNodes().back()" << std::endl;
    g.erase_node( *( g.getNodes().back() ) );

    g.rollcall();

    std::cout << "PhysSpace, reflecting the graph:" << std::endl;
    PhysSpace R3(g);

    // WINDOW

    sf::RenderWindow window(sf::VideoMode(500, 500), "Test window (press ESC)");
    window.setFramerateLimit(60);
    sf::CircleShape circle(200.f);
    circle.setFillColor(sf::Color::White);
    circle.setPosition(50.f, 50.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }

        window.clear(sf::Color(3, 16, 25));

        window.draw(circle);
        
        window.display();
    }

    return 0;
}