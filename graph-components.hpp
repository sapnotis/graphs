#ifndef _GRAPH_LOGIC_
#define _GRAPH_LOGIC_

#include <deque>
#include <string>

class Graph;
class Node;
class Edge;

class Graph
{
private:
    std::deque<Node> nodes;
    std::deque<Edge> edges;
public:
    Graph();
    ~Graph();

    bool is_corrupted(); // full debug: all objects know their connections, no edges lead away

    Node& emplace_node(std::deque<int> values);
    Edge& emplace_edge(Node* f, Node* s);

    void erase_node(const Node& node);
    void erase_edge(const Edge& edge);
        // argument is reference so user may
        // delete node/edge by its values
        // ( funcion compares members )

    std::deque<Node*> getNodePtrs();
    // std::deque<Edge> getEdges();

    void rollcall();
};

class Node
{
private:
    std::deque<int> values;
    std::deque<Edge*> edges;
public:
    Node(std::deque<int> values);
    ~Node(); // destroy belonging edges

    bool operator==(const Node& other) const { return ( values == other.values ); }

    void add_edge(Edge* edge);

    std::deque<int> getValues() { return values; };
    std::deque<Edge*> getEdges() { return edges; };
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