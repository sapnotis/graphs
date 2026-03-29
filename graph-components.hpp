#ifndef _GRAPH_LOGIC_
#define _GRAPH_LOGIC_

#include <deque>
#include <vector>
#include <string>

class Graph;
class Node;

class Graph
{
private:
    const bool allow_equal_nodes; // meant to stay false (one node represents one state of values)
    const bool allow_multiple_edges; // meant to stay false (different moves of the same result are impossible)
    std::deque<Node> nodes;
public:
    Graph();
    ~Graph();

    bool is_corrupted() const; // full debug: all objects know their connections, no edges lead away

    Node* emplace_node(std::vector<int> values);
    void emplace_edge(Node* f, Node* s);

    void erase_node(const Node& node);
    void erase_edge(Node* f, Node* s);

    std::vector<Node*> getNodes();
    Node* findNode(std::vector<int> values);

    void rollcall();
};

class Node
{
private:
    std::vector<int> values;
    std::vector<Node*> edges;
public:
    bool checked;
    
    Node(std::vector<int> values);
    ~Node(); // destroys belonging edges

    bool operator==(const Node& other) const { return ( values == other.values ); }

    void add_edge(Node* node);
    bool find_edge(Node* node);
    void forget_edge(Node* node);

    std::vector<int> getValues() { return values; };
    std::vector<Node*> getEdges() { return edges; };
};

#endif