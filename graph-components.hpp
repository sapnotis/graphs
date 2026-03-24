#ifndef _GRAPH_LOGIC_
#define _GRAPH_LOGIC_

#include <deque>
#include <vector>
#include <string>

class Graph;
class Node;
class Edge;

class Graph
{
private:
    const bool allow_equal_nodes; // meant to stay false (one node represents one state of values)
    const bool allow_multiple_edges; // meant to stay false (different moves of the same result are impossible)
    std::deque<Node> nodes;
    std::deque<Edge> edges;
public:
    Graph();
    ~Graph();

    bool is_corrupted() const; // full debug: all objects know their connections, no edges lead away

    Node* emplace_node(std::vector<int> values);
    Edge* emplace_edge(Node* f, Node* s);

    void erase_node(const Node& node);
    void erase_edge(const Edge& edge);
        // argument is reference so user may delete node/edge by its values
        // erase_smth(smth* ptr) override may be added ?

    std::vector<Node*> getNodes();
    std::vector<Edge*> getEdges();

    void rollcall();
};

class Node
{
private:
    std::vector<int> values;
    std::vector<Edge*> edges;
public:
    Node(std::vector<int> values);
    ~Node(); // destroys belonging edges

    bool operator==(const Node& other) const { return ( values == other.values ); }

    Edge* add_edge(Edge* edge);
    void remove_edge(const Edge& edge);

    std::vector<int> getValues() { return values; };
    std::vector<Edge*> getEdges() { return edges; };
};

class Edge
{
private:
    bool directional;
    Node* first;
    Node* second;
public:
    Edge(Node* f, Node* s);
    ~Edge();

    bool operator==(const Edge& other) const { return ( first == other.first && second == other.second ); }

    Node* getFirst() { return first; };
    Node* getSecond() { return second; };
};

#endif