#include <iostream>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    g.emplace_node({1, 1});
    g.emplace_edge( {1, 1}, g.emplace_node({1, 2}) );
    g.emplace_edge( {1, 2}, g.emplace_node({1, 3}) );
    g.emplace_edge( g.emplace_node({2, 1}), {1, 1} );

    g.rollcall();

    // WINDOW

    g.update_nodes();

    sf::Clock clock;
    clock.restart();
    
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Test window (press ESC)");
    window.setFramerateLimit(50);

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

        // if ( clock.getElapsedTime().asMilliseconds() > 500 ) {
            // clock.restart();
            g.update_nodes();
        // }
        g.display(window, 100);
        
        window.display();
    }

    return 0;
}