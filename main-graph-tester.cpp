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

    sf::Clock clock;
    clock.restart();

    // sf::Font font;
    // if ( !font.loadFromFile("arialmt.ttf") )
    //     return -1;
        
    // sf::Text text;
    // text.setFont(font);
    // text.setCharacterSize(40);
    // text.setFillColor(sf::Color::White);
    // text.setPosition(20.f, 20.f);
    
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Test window (press ESC)");
    window.setFramerateLimit(20);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            g.yaw += 0.02;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            g.yaw -= 0.02;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            g.pitch += 0.02;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            g.pitch -= 0.02;

        window.clear(sf::Color(3, 16, 25));

        if ( clock.getElapsedTime().asMilliseconds() >= 200 ) {
            clock.restart();
            g.update_nodes();
        }
        
        g.display(window, 100);
        
        window.display();
    }

    return 0;
}