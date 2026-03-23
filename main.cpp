#include <iostream>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl << std::endl;

    Graph g;

    g.emplace_node({1});
    g.emplace_node({2});
    g.emplace_node({3});
    g.emplace_node({4});
    // g.emplace_edge( g.getNodes()[0], g.getNodes()[1] );

    g.rollcall();

    g.erase_node(g.getNodes()[0]);

    g.rollcall();

    return 0;
}