#include <iostream>
#include <string>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    g.emplace_node( {0, 0} );

    g.emplace_edge( {0, 0}, g.emplace_node({1, 1}) );
    g.emplace_edge( g.emplace_node({1, 2}), {1, 1} );

    g.emplace_edge( {0, 0}, g.emplace_node({2, 1}) );
    g.emplace_edge( {2, 1}, g.emplace_node({2, 2}) );

    g.emplace_edge( {0, 0}, g.emplace_node({3, 1}) );
    g.emplace_edge( {3, 1}, g.emplace_node({3, 2}) );

    g.emplace_edge( {0, 0}, g.emplace_node({4, 1}) );
    g.emplace_edge( {4, 1}, g.emplace_node({4, 2}) );

    g.rollcall();

    // WINDOW
    // return 0;

    g.update_nodes();

    // sf::Clock clock;
    // clock.restart();
    
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Test window (press ESC)");
    window.setFramerateLimit(60);

    const float delta_angle = 0.03;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::R)
                    g.resetYawPitch();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            g.addYaw( delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            g.addYaw( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            g.addPitch( delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            g.addPitch( - delta_angle );

        window.clear(sf::Color(3, 16, 25));

        // if ( clock.getElapsedTime().asMilliseconds() >= 2000 ) {
            // clock.restart();
            // g.update_nodes();
        // }
        
        g.display(window, 200);
        
        window.display();
    }

    return 0;
}