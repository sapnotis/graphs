#ifndef _GRAPH_LOGIC_
#define _GRAPH_LOGIC_

#include <vector>
#include <string>

class Graph;
class Node;
class Edge;

class Graph
{
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
public:
    Graph();
    ~Graph();

    bool is_corrupted(); // full debug: all objects know their connections, no edges lead away

    Node& emplace_node(std::vector<int> values);
    Edge& emplace_edge(Node& f, Node& s);

    void erase_node(const Node& node);
    void erase_edge(const Edge& edge);

    std::vector<Node> getNodes();
    std::vector<Edge> getEdges();

    void rollcall();
};

class Node
{
private:
    std::vector<int> values;
    std::vector<Edge*> edges;
public:
    Node(std::vector<int> values);
    ~Node(); // destroy belonging edges

    bool operator==(const Node& other) const;

    std::vector<int> getValues() { return values; };
    std::vector<Edge*> getEdges() { return edges; };

    // void add_edge();
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

    bool operator==(const Edge& other) const;

    Node* getFirst() { return first; };
    Node* getSecond() { return second; };
};

#endif