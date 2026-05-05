#include <iostream>
using std::cin, std::cout, std::endl;
#include "board.hpp"
#include "combinator.hpp"
#include "graph-components.hpp"

#include <cmath>
#include <string>
#include <sstream>

int main() {

    // ============================== BOARD ==============================

    cout << "\nPreparing board..." << endl;

    int width = 0, height = 0;
    vector<Piece> pieces;
    if ( !select_board(width, height, pieces) )
        return 1;

    Board board(width, height, pieces);

    std::vector<int> first_node{};
    for ( Piece p : pieces ) {
        first_node.push_back(p.x);
        first_node.push_back(p.y);
    }

    if ( !board.isValid(first_node) ) {
        cout << "Invalid board setup! exit" << endl;
        return 1;
    }

    // ============================== GRAPH ==============================

    cout << "\nPreparing graph..." << endl;

    Graph graph;
    graph.emplace_node(first_node);
    board_to_graph(board, graph);

    cout << "\nGraph contains " << graph.getNodes().size() << " nodes" << endl;

    // ============================== WINDOW ==============================
    
    cout << "\nPreparing window..." << endl;

    const int FPS = 60;
    const float delta_angle = 0.05;
    int frame = 0;
    
    sf::RenderWindow window(sf::VideoMode( 1600, 900 ), "Board2graph");
    window.setFramerateLimit(FPS);

    sf::Font font;
    if ( !font.loadFromFile("arialmt.ttf") )
        return 1;

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

    cout << "\nRunning window..." << endl;

    cout << "\nUse buttons:" << endl;
    cout << "  Esc to exit" << endl;
    cout << "  R to Reset Yaw/Pitch" << endl;
    cout << "  S to Shake" << endl;
    cout << "  M to change Mode..." << endl;
    cout << "In \"node travelling\" mode use Tab to select next node and Enter to go there." << endl;

    for ( Node* tmp : graph.getNodes() )
        tmp->set_coords( rnd_xyz_direction(20.f*std::cbrt (graph.getNodes().size())) );
    
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
                        tmp->set_coords( rnd_xyz_direction(20.f*std::cbrt (graph.getNodes().size())) );

                if (event.key.code == sf::Keyboard::M) {
                    graph.set_selected_neighbour(-1);
                    if ( graph.get_selected_node() )
                        graph.set_selected_node(nullptr);
                    else
                        graph.set_selected_node( graph.findNode(first_node) );
                }

                if (event.key.code == sf::Keyboard::Tab)
                    if ( graph.get_selected_node() )
                        if ( graph.get_selected_node()->getEdges().size() )
                            graph.set_selected_neighbour(
                                ( graph.get_selected_neighbour() + 1 ) % ( graph.get_selected_node()->getEdges().size() )
                            );

                if (event.key.code == sf::Keyboard::Enter)
                    if ( graph.get_selected_node() && graph.get_selected_neighbour() != -1 ) {

                        graph.set_selected_node( graph.get_selected_node()->getEdges()[graph.get_selected_neighbour()] );
                        if ( graph.get_selected_node()->getEdges().size() )
                            graph.set_selected_neighbour( 0 );
                        else
                            graph.set_selected_neighbour( -1 );
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

        debug_text.setString(
            "Inputs:\nEsc, R(eset), S(hake), M(ode).\nTab & Enter for travelling.\n\nDebug:\nFPS limit: "
            + std::to_string(FPS)
            + "\nLag ratio: " + std::to_string( LagRatio )
            + "\nNodes: " + std::to_string(graph.getNodes().size())
            + "\nvelocity_limit: " + std::to_string(Node::velocity_limit)
            + "\ninteract_koef: " + std::to_string(Node::interact_koef)
            + "\nvel_multiplier: " + std::to_string(Node::vel_multiplier) );

        std::stringstream ss;
        if ( graph.get_selected_node() )
            for ( int i : graph.get_selected_node()->getValues() )
                ss << i << " ";
        node_text.setString( ss.str() );
        node_text.setPosition(window.getSize().x / 2 + 10, window.getSize().y / 2 + 10);

        window.draw(debug_text);
        window.draw(node_text);
        graph.display(window);
        
        window.display();

        graph.tick();
    }

    return 0;
}