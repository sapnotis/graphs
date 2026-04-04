#include <iostream>
#include <string>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    g.emplace_edge( g.emplace_node({1}), g.emplace_node({2}) );
    g.emplace_edge( g.emplace_node({3}), g.findNode({1}) );
    g.emplace_edge( g.findNode({2}), g.findNode({3}) );

    // WINDOW
    // return 0;
    
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
                if (event.key.code == sf::Keyboard::S)
                    for ( Node* tmp : g.getNodes() )
                        tmp->set_coords(xyz_rnd_direction(1.f));
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
        
        g.update_nodes();
        g.display(window);
        
        window.display();
    }

    return 0;
}