#include <iostream>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    g.emplace_node({1, 1});
    g.emplace_edge( g.findNode({1, 1}), g.emplace_node({1, 2}) );
    g.emplace_edge( g.findNode({1, 2}), g.emplace_node({1, 3}) );
    g.emplace_edge( g.findNode({1, 1}), g.emplace_node({2, 1}) );

    g.rollcall();

    std::cout << "erase_node getNodes().front()" << std::endl; 
    g.erase_node( *( g.getNodes().front() ) );

    g.rollcall();

    std::cout << "erase_node getNodes().back()" << std::endl;
    g.erase_node( *( g.getNodes().back() ) );

    g.rollcall();

    return 0;
}