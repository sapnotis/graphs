#ifndef _COMBINATOR_LOGIC_
#define _COMBINATOR_LOGIC_

#include "graph-components.hpp"
#include "board.hpp"

void board_to_graph(Board& board, Graph& graph);

void test_graph_torus(Graph& graph);
void test_graph_beating_thing(Graph& graph);

#endif