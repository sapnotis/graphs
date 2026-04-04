#include <iostream>
#include <string>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    for ( int i=1; i<7; i++ )
        g.emplace_node( {i} );
    
    for ( int i : {5, 6} ) {
        g.emplace_edge( {i}, {1} );
        g.emplace_edge( {i}, {2} );
        g.emplace_edge( {i}, {3} );
        g.emplace_edge( {i}, {4} );
    }

    g.emplace_edge( {1}, {2} );
    g.emplace_edge( {2}, {3} );
    g.emplace_edge( {3}, {4} );
    g.emplace_edge( {4}, {1} );

    g.rollcall();

    // WINDOW
    // return 0;

    // sf::Clock clock;
    // clock.restart();
    
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Test window (press ESC)");
    window.setFramerateLimit(50);

    const float delta_angle = 0.05;

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
        
        g.update_nodes();
        g.display(window, 400);
        
        window.display();
    }

    return 0;
}