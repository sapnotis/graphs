#include "combinator.hpp"
using std::vector;
#include <queue>
using std::queue;

void board_to_graph(Board& board, Graph& graph)
{

    if ( graph.getNodes().empty() ) {
        std::cout << "(!) Graph has no starting point" << std::endl;
        return;
    }

    vector<int> startState = graph.getNodes().front()->getValues();

    if ( startState.size() % 2 ) {
        std::cout << "(!) Odd state size (how tf did you even get this way)" << std::endl;
        return;
    }

    const int numBlocks = startState.size() / 2;

    queue<std::vector<int>> queue;
    queue.push(startState);

    while (!queue.empty()) {

        if ( graph.getNodes().size() >= 800 ) {
            std::cout << "(!) Too many nodes! Interrupted graph processing" << std::endl;
            break;
        }

        vector<int> currentState = queue.front();
        queue.pop();

        Node* currentNode = graph.findNode(currentState);
        if (currentNode == nullptr || currentNode->status != UNKNOWN )
            continue;

        currentNode->status = ORDINARY;

        for (int block_num = 0; block_num < numBlocks; block_num++) {
            int x = currentState[2 * block_num];
            int y = currentState[2 * block_num + 1];

            int dx[4] = {1, -1, 0, 0};
            int dy[4] = {0, 0, 1, -1};

            for (int dir = 0; dir < 4; dir++) {
                
                vector<int> nextState = currentState;

                nextState[2 * block_num] = x + dx[dir];
                nextState[2 * block_num + 1] = y + dy[dir];

                vector<int> resultState = board.result_state_of(nextState);

                if ( resultState.empty() ) {
                    continue;
                }

                if ( !graph.findNode(resultState) )
                    graph.emplace_node(resultState);

                if ( !graph.findEdge(currentState, resultState) )
                    graph.emplace_edge(currentState, resultState);

                if ( graph.findNode(resultState)->status == UNKNOWN )
                    queue.push(resultState);
            }
        }
    }

    graph.findNode(startState)->status = START;
}