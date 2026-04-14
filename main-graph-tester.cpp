#include <iostream>
#include <string>
#include "graph-components.hpp"
#include <chrono>
#include <cmath>

void shake(Graph& g) {
    for ( Node* tmp : g.getNodes() )
        tmp->set_coords(rnd_xyz_direction( 20 * std::cbrt ( g.getNodes().size() ) ));
}

int main() {
    std::cout << "Preparing graph..." << std::endl;

    Graph g;

    // square
    // g.emplace_node({1, 1});
    // g.emplace_node({1, 2});
    // g.emplace_node({2, 1});
    // g.emplace_node({2, 2});

    // g.emplace_edge( {1, 1}, {1, 2} );
    // g.emplace_edge( {2, 1}, {2, 2} );
    // g.emplace_edge( {1, 1}, {2, 1} );
    // g.emplace_edge( {1, 2}, {2, 2} );

    // big thing
    const int low = 3;
    const int high = 10;

    for ( int i=low; i<=high; i++ )
        for ( int j=low; j<=high; j++ )
            for ( int k=low; k<=high; k++ )
                g.emplace_node({i, j ,k});
    
    for ( int i=low; i<=high; i++ )
        for ( int j=low; j<=high; j++ )
            for ( int k=low; k<=high; k++ ) {
                g.emplace_edge({i, j , k}, {i+1, j , k});
                g.emplace_edge({i, j , k}, {i, j+1 , k});
                g.emplace_edge({i, j , k}, {i, j , k+1});
            }

    for ( int i=5; i<=8; i++ )
        for ( int j=5; j<=8; j++ )
            for ( int k=low; k<=high; k++ )
                g.erase_node({i, j , k});

    std::cout << "Graph contains " << g.getNodes().size() << " nodes;" << std::endl;

    // return 0;
    // WINDOW

    std::cout << "Preparing window..." << std::endl;

    const int FPS = 50;
    int frame = 0;
    const float delta_angle = 0.05;
    
    sf::RenderWindow window(sf::VideoMode( 1600, 900 ), "Graph");
    window.setFramerateLimit(FPS);

    sf::Font font;
    if ( !font.loadFromFile("arialmt.ttf") )
        return -1;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(20.f, 20.f);

    sf::Clock clock;
    clock.restart();

    std::cout << "Running window..." << std::endl;

    std::cout << "  - Arrows to rotate;\n  - S to Shake;\n  - R to Reset yaw/pitch;\n  - N to select random Node;\n  - Esc to quit." << std::endl;
    
    shake(g);
    
    float LagRatio = 1;

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
                    shake(g);
                if (event.key.code == sf::Keyboard::N) {
                    if ( g.get_selected_node() )
                        g.set_selected_node(nullptr);
                    else
                        g.set_selected_node( g.getNodes()[rnd_number(0, g.getNodes().size())] );
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            g.addYaw( delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            g.addYaw( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            g.addPitch( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            g.addPitch( delta_angle );

        window.clear(sf::Color(3, 16, 25));

        frame++;

        if ( frame >= FPS ) {
            frame = 0;
            LagRatio = 0.001f * clock.restart().asMilliseconds();
        }

        text.setString( "Lag ratio: " + std::to_string( LagRatio )
            + "\nNodes: " + std::to_string(g.getNodes().size())
            + "\nvelocity_limit: " + std::to_string(Node::velocity_limit)
            + "\ninteract_koef: " + std::to_string(Node::interact_koef)
            + "\nvel_multiplier: " + std::to_string(Node::vel_multiplier) );
        
        window.draw(text);

        g.update_nodes();
        g.display(window);
        
        window.display();
    }

    return 0;
}