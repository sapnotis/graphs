#include "combinator.hpp"

void board_to_graph(Board& board, Graph& graph)
{
    // построение графа graph состояний доски board
};

void test_graph_torus(Graph& graph)
{
    // torus
    for ( int i=0; i<=20; i++ )
    for ( int j=0; j<=10; j++ )
        graph.emplace_node( {i, j} );
    
    for ( int i=0; i<=20; i++ )
    for ( int j=0; j<=10; j++ ) {
        graph.emplace_edge( {i, j}, {(i+1)%21, j} );
        graph.emplace_edge( {i, j}, {i, (j+1)%11} );
    }
};

void test_graph_beating_thing(Graph& graph)
{
    // beating thing
    for ( int i=1; i<=10; i++ )
    for ( int j=1; j<=10; j++ )
    for ( int k=1; k<=10; k++ )
        graph.emplace_node({i, j ,k});
    
    for ( int i=1; i<=10; i++ )
    for ( int j=1; j<=10; j++ )
    for ( int k=1; k<=10; k++ ) {
        graph.emplace_edge({i, j , k}, {i+1, j , k});
        graph.emplace_edge({i, j , k}, {i, j+1 , k});
        graph.emplace_edge({i, j , k}, {i, j , k+1});
    }

    for ( int i=3; i<=5; i++ )
    for ( int j=3; j<=5; j++ )
    for ( int k=1; k<=10; k++ )
        graph.erase_node({i, j , k});
};