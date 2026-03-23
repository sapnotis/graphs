#include <iostream>
#include "graph-components.hpp"

int main() {
    std::cout << "When I grow up I will become a graph visualiser!" << std::endl;

    Graph g;

    Node* tmpA = new Node("Alk", {1, 2});
    Node* tmpB = new Node("Bru", {2, 5, 6});

    g.push_node(*tmpA);
    g.push_node(*tmpB);

    delete tmpA;
    delete tmpB;

    g.rollcall();

    return 0;
}