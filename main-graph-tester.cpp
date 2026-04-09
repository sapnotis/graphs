#include <iostream>
#include <string>
#include "graph-components.hpp"
#include <chrono>

int main() {
    std::cout << "Preparing graph..." << std::endl;

    Graph g;

    // int count = 1;

    // for ( int i = 1; i<=3; i++ )
    //     for ( int j = 1; j<=3; j++ )
    //         g.emplace_node( {i, j} );

    // for ( int i = 1; i<=3; i++ )
    //     for ( int j = 1; j<=3; j++ ) {
    //         g.emplace_edge( {i, j}, {i+1, j} );
    //         g.emplace_edge( {i, j+1}, {i, j} );       
    //     }

    // g.erase_node( g.findNode({2, 2}) ); ???

    // g.rollcall();

    for ( int i=1; i<=7; i++ )
        for ( int j=1; j<=7; j++ )
            for ( int k=1; k<=7; k++ )
                g.emplace_node({i, j ,k});
    
    for ( int i=1; i<=7; i++ )
        for ( int j=1; j<=7; j++ )
            for ( int k=1; k<=7; k++ ) {
                g.emplace_edge({i, j , k}, {i+1, j , k});
                g.emplace_edge({i, j , k}, {i, j+1 , k});
                g.emplace_edge({i, j , k}, {i, j , k+1});
            }

    std::cout << "Preparing window..." << std::endl;
    
    // WINDOW

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

    std::cout << "  - Arrows to rotate;\n  - S to Shake;\n  - Esc to quit." << std::endl;

    float LagRatio;

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
                        tmp->set_coords(xyz_rnd_direction( g.getNodes().size() ));
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