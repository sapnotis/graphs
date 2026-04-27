#include <iostream>
#include "graph-components.hpp"
#include "combinator.hpp"
// #include "board.hpp or something"

#include <chrono>
#include <cmath>
#include <sstream>
#include <string>

int main() {

    // ============================== ДОСКА ==============================
    
    std::cout << "Preparing board..." << std::endl;

    // const int width = 3; // ширина
    // const int height = 3; // высота
    // const int N = 4; // кол-во блоков
    // int[N][2] blocks = { {1, 1}, {1, 1}, {1, 1}, {2, 2} }; // размеры блоков

    // Board board(width, height, N, blocks); // инициализация доски

    // ============================== ГРАФ ==============================

    std::cout << "Preparing graph..." << std::endl;

    Graph graph;
    // graph.emplace_node( {...} ); // стартовая вершина
    // board_to_graph(board, graph); // построение графа

    test_graph_torus(graph); // тестовый граф, не отражающий доску, заместо board_to_graph

    std::cout << "Graph contains " << graph.getNodes().size() << " nodes;" << std::endl;

    // ============================== ОКНО ==============================
    
    std::cout << "Preparing window..." << std::endl;

    const int FPS = 60;
    int frame = 0;
    const float delta_angle = 0.05;
    
    sf::RenderWindow window(sf::VideoMode( 1600, 900 ), "Graph");
    window.setFramerateLimit(FPS);

    sf::Font font;
    if ( !font.loadFromFile("arialmt.ttf") )
        return -1;

    sf::Text debug_text;
    debug_text.setFont(font);
    debug_text.setCharacterSize(20);
    debug_text.setFillColor(sf::Color::White);
    debug_text.setPosition(20.f, 20.f);

    sf::Text node_text;
    node_text.setFont(font);
    node_text.setCharacterSize(14);
    node_text.setFillColor(sf::Color::White);

    sf::Clock clock;
    clock.restart();

    std::cout << "Running window..." << std::endl;
 
    for ( Node* tmp : graph.getNodes() )
        tmp->set_coords(rnd_xyz_direction( 20 * std::cbrt ( graph.getNodes().size() ) ));
    
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
                    graph.resetYawPitch();
                if (event.key.code == sf::Keyboard::S)
                    for ( Node* tmp : graph.getNodes() )
                        tmp->set_coords(rnd_xyz_direction( 20 * std::cbrt ( graph.getNodes().size() ) ));
                if (event.key.code == sf::Keyboard::N) {
                    graph.set_selected_neighbour(-1);
                    if ( graph.get_selected_node() ) graph.set_selected_node(nullptr);
                    else graph.set_selected_node( graph.getNodes()[rnd_number(0, graph.getNodes().size()-1)] );
                }
                if (event.key.code == sf::Keyboard::Tab)
                    if ( graph.get_selected_node() )
                        if ( graph.get_selected_node()->getEdges().size() )
                            graph.set_selected_neighbour(
                                (graph.get_selected_neighbour() + 1) % graph.get_selected_node()->getEdges().size()
                            );
                if (event.key.code == sf::Keyboard::Enter) {
                    if ( graph.get_selected_node() && graph.get_selected_neighbour() != -1 ) {

                        graph.set_selected_node( graph.get_selected_node()->getEdges()[graph.get_selected_neighbour()] );
                        if ( graph.get_selected_node()->getEdges().size() )
                            graph.set_selected_neighbour( 0 );
                        else
                            graph.set_selected_neighbour( -1 );
                    }
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            graph.addYaw( delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            graph.addYaw( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            graph.addPitch( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            graph.addPitch( delta_angle );

        window.clear(sf::Color(3, 16, 25));

        frame++;
        if ( frame >= FPS/5 ) {
            frame = 0;
            LagRatio = 0.005f * clock.restart().asMilliseconds();
        }

        debug_text.setString( "FPS limit: " + std::to_string(FPS)
            + "\nLag ratio: " + std::to_string( LagRatio )
            + "\nNodes: " + std::to_string(graph.getNodes().size())
            + "\nvelocity_limit: " + std::to_string(Node::velocity_limit)
            + "\ninteract_koef: " + std::to_string(Node::interact_koef)
            + "\nvel_multiplier: " + std::to_string(Node::vel_multiplier) );

        node_text.setPosition(window.getSize().x / 2 + 10, window.getSize().y / 2 + 10);
        std::stringstream ss;
        if ( graph.get_selected_node() )
            for ( int i : graph.get_selected_node()->getValues() )
                ss << i << " ";
        else
            ss << 0;
        node_text.setString( ss.str() );

        window.draw(debug_text);
        window.draw(node_text);

        graph.tick();
        graph.display(window);
        
        window.display();
    }

    return 0;
}