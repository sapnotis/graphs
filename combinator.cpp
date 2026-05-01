#include "combinator.hpp"
#include <queue>

void board_to_graph(Board& board, Graph& graph)
{
    std::vector<Node*> nodes = graph.getNodes();
    if (nodes.empty()) return;
    std::vector<int> startState = nodes[0]->getValues();

    std::queue<std::vector<int>> queue;
    queue.push(startState);

    while (!queue.empty()) {

        if ( graph.getNodes().size() >= 800 ) {
            std::cout << "Too many nodes! Further graph generation interrupted" << std::endl;
            return;
        }

        std::vector<int> currentState = queue.front();
        queue.pop();

        Node* currentNode = graph.findNode(currentState);
        if (currentNode == nullptr || currentNode->checked) {
            continue;
        }

        currentNode->checked = true;

        int numBlocks = currentState.size() / 2;

        for (int block = 0; block < numBlocks; block++) {
            int x = currentState[2 * block];
            int y = currentState[2 * block + 1];

            int dx[] = {1, -1, 0, 0};
            int dy[] = {0, 0, 1, -1};

            for (int dir = 0; dir < 4; dir++) {
                std::vector<int> nextState = currentState;
                nextState[2 * block] = x + dx[dir];
                nextState[2 * block + 1] = y + dy[dir];

                if ( !board.isValid(nextState) ) {
                    continue;
                }

                if ( !graph.findNode(nextState) ) {
                    graph.emplace_node(nextState);
                }
                
                graph.emplace_edge(currentState, nextState);

                Node* nextNode = graph.findNode(nextState);
                // находим указатель на новую вершину. Вершины добавляются в граф в момент обнаружения. Указатель нужен, чтобы проверить checked и решить, нужно ли обрабатывать эту вершину позже
                if (nextNode != nullptr && !nextNode->checked) {
                    queue.push(nextState);
                }
            }
        }
    }
}

void test_graph_torus(Graph& graph)
{
    for (int i = 0; i <= 20; i++)
    for (int j = 0; j <= 10; j++)
        graph.emplace_node({i, j});

    for (int i = 0; i <= 20; i++)
    for (int j = 0; j <= 10; j++) {
        graph.emplace_edge({i, j}, {(i+1)%21, j});
        graph.emplace_edge({i, j}, {i, (j+1)%11});
    }
}

void test_graph_beating_thing(Graph& graph)
{
    for (int i = 1; i <= 10; i++)
    for (int j = 1; j <= 10; j++)
    for (int k = 1; k <= 10; k++)
        graph.emplace_node({i, j, k});

    for (int i = 1; i <= 10; i++)
    for (int j = 1; j <= 10; j++)
    for (int k = 1; k <= 10; k++) {
        graph.emplace_edge({i, j, k}, {i+1, j, k});
        graph.emplace_edge({i, j, k}, {i, j+1, k});
        graph.emplace_edge({i, j, k}, {i, j, k+1});
    }

    for (int i = 3; i <= 5; i++)
    for (int j = 3; j <= 5; j++)
    for (int k = 1; k <= 10; k++)
        graph.erase_node({i, j, k});
}
