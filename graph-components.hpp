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

    void push_node(const Node& node);
    void push_edge(const Edge& edge);
    void erase_node(const Node& node);
    void erase_edge(const Edge& edge);
    void rollcall();
};

class Node
{
private:
    std::vector<Edge*> edges;
    std::string name;
    std::vector<int> values;
public:
    Node(std::string name, std::vector<int> values);
    ~Node(); // destroy belonging edges

    void add_edge(Edge* edge);
    std::string getName() { return name; };
    std::vector<int> getValues() { return values; };
};

class Edge
{
private:
    bool directional;
    Node* first;
    Node* second;
    // std::vector<int> delta_values;   represents move (?)
public:
    Edge(Node* f, Node* s);
    ~Edge();
};

#endif