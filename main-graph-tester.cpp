#include <iostream>
#include <string>
#include "graph-components.hpp"
#include <chrono>

int main() {
    std::cout << "Preparing graph..." << std::endl;

    Graph g;

    g.emplace_node({1});

    int count = 1;
    for ( unsigned int i=0; i<10; i++ ) {
        g.emplace_edge( {count}, g.emplace_node({count+1}) );
        g.emplace_edge( {2-count}, g.emplace_node({1-count}) );
        count++;
    }

    std::cout << "Preparing window..." << std::endl;
    
    // WINDOW

    const int FPS = 50;
    int frame = 0;
    const float delta_angle = 0.05;
    
    sf::RenderWindow window(sf::VideoMode( 1600, 900 ), "SFML Window");
    window.setFramerateLimit(FPS);

    sf::Font font;
    if ( !font.loadFromFile("arialmt.ttf") )
        return -1;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setPosition(20.f, 20.f);

    sf::Clock clock;
    clock.restart();

    std::cout << "Running window..." << std::endl;

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
                        tmp->set_coords(xyz_rnd_direction(5));
                if (event.key.code == sf::Keyboard::A) {
                    g.emplace_edge( {count}, g.emplace_node({count+1}) );
                    g.emplace_edge( {2-count}, g.emplace_node({1-count}) );
                    count++;
                }
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

        frame++;

        if ( frame >= FPS ) {
            frame = 0;
            float LagRatio = 0.001f * clock.restart().asMilliseconds();
            text.setString( "Lag ratio:\n" + std::to_string( LagRatio )
                + "\nNodes:\n" + std::to_string(2*count-1) );
        }
        
        window.draw(text);

        g.update_nodes();
        g.display(window);
        
        window.display();
    }

    return 0;
}