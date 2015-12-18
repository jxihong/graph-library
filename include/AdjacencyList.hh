#ifndef _ADJLIST_HH_
#define _ADJLIST_HH_

#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

#include "Node.hh"
#include "Edge.hh"

template <typename T>
class AdjacencyList;

/*
 * Forward declaration of friend methods of the class
 */
template<typename T>
bool hasNegativeCycle(const AdjacencyList<T> &g);

/*
 * Describes the adjacency-list structure of representing a 
 * graph data structure
 */
template <typename T>
class AdjacencyList {

private:
  std::map<int, std::list<EdgePtr<T> > > _graph; // uses node ID as key for outgoing edges
  std::vector<Node<T>* > _nodes; // keeps track of all nodes
 
  bool _isDirected;

  struct DeleteObject  { 
    template<typename type> 
    void operator()(const type *ptr) const { 
      delete ptr; 
    } 
  };
  
  // Function object to determine if edge has specified node ID
  struct HasNode : public std::unary_function<int, bool> { 
    HasNode(int id) : _id(id) {}
    int _id;
    bool operator()(EdgePtr<T> e) { 
      return ((e->getEnd()->getID() == _id) || (e->getStart()->getID() == _id));
    }
  };
  
  // Function object that resets state and weight of a node
  struct ResetNode : public std::unary_function<void, void> {
    void operator() (Node<T> *n) {
      n->setState(NOT_VISITED);
      n->setWeight(Node<T>::INFINITY);
    }
  };

  AdjacencyList() = delete; // Removes default constructor

public:
  // Empty graph
  AdjacencyList(bool directed) : _graph(), _nodes(), _isDirected(directed) {}

  // Constructs graph from file input of the following format:
  // numNodes numEdges
  // startID endID edgeWeight // Edge 1
  // ...
  AdjacencyList(std::istream &input, bool directed);
  ~AdjacencyList();
  
  // Returns number of nodes
  inline size_t size() { return _nodes.size(); }

  // Returns node with the specified ID
  inline Node<T> * node(int id) const { return _nodes[id]; }
  
  // Returns whether the graph is directed
  inline bool isDirected() const { return _isDirected; }

  // Connects two existing nodes
  void addEdge(int from, int to, T weight);
  
  void addNode(int id);

  void removeEdge(int from, int to);

  void removeNode(int id);
  
  // Computes number of incoming and outgoing edges
  size_t inDegree(const Node<T> *n) const;
  size_t outDegree(const Node<T> *n) const;

  // Returns vector of all outgoing edges from a node
  const std::list<EdgePtr<T> > & adjacent(const Node<T> *n) const;
  
  // Sets all nodes as not visited with weight INFINITY
  void reset();
  
  // Prints the graph in the following format:
  // NodeID: Edge Edge ... // Node 1 and its Outgoing Edges
  // ...
  friend std::ostream & operator<<(std::ostream &os, const AdjacencyList<T> &g) {
    for (const auto& i : g._graph) {
      os << i.first << ":";
      for (auto& edge : i.second) {
	os << "(" 
	   << edge->getStart()->getID() << ", " 
	   << edge->getEnd()->getID() << ", "
	   << edge->getWeight() << ")";
      }
      os << std::endl;
    }
    return os;
  }

};

template<typename T>
AdjacencyList<T>::AdjacencyList(std::istream &input, bool directed) 
  :  _graph(), _nodes(), _isDirected(directed) {
  
  size_t nNodes, nEdges;
  input >> nNodes >> nEdges;
  
  _nodes.resize(nNodes, nullptr);

  int from, to; // id's of the two nodes
  T weight;
  while (input >> from >> to >> weight) {
    addNode(from);
    addNode(to);
    
    addEdge(from, to, weight);
  }
}

template<typename T> 
AdjacencyList<T>::~AdjacencyList() {
  for_each(_nodes.begin(), _nodes.end(), DeleteObject());
  _nodes.clear();

  _graph.clear();
}

template<typename T>
inline void AdjacencyList<T>::addEdge(int from, int to, T weight) {
  Edge<T> *e = new Edge<T>(_nodes[from], _nodes[to] , weight);
  _graph[from].push_back(EdgePtr<T>(e));

  if (!_isDirected) { // adds undirected edge
    Edge<T> *e_rev = new Edge<T>(_nodes[to], _nodes[from], weight);
    _graph[to].push_back(EdgePtr<T>(e_rev));
  }
  else {
    _graph[to]; // show that node exists
  }
}

template<typename T>
inline void AdjacencyList<T>::addNode(int id) {
  if (id >=(int) _nodes.size()) { // if id is out of vector range
    _nodes.resize(id+1, nullptr);
  }
  if (_nodes[id] == nullptr) { // does nothing if node already exists
    Node<T> *new_node = new Node<T>(id);
    _nodes[id] = new_node;
    _graph[id]; // show that node exists
  } 
}

template<typename T>
inline void AdjacencyList<T>::removeEdge(int from, int to) {
  _graph[from].remove_if(HasNode(to));
  if (!_isDirected) {
    _graph[to].remove_if(HasNode(from));
  }
}

template<typename T>
inline void AdjacencyList<T>::removeNode(int id) {
  if (!_isDirected) {
    for (auto& edge : _graph[id]) {
       removeEdge(edge->getEnd()->getID(), id);
    }
  }
  _graph.erase(id);

  delete _nodes[id]; // frees memory allocated by node
  _nodes[id] = nullptr;
}

template<typename T>
inline size_t AdjacencyList<T>::inDegree(const Node<T> *n) const {
  size_t deg = 0;
  for (const auto& i : _graph) {
    for (auto& edge : i.second) {
      if (edge->getEnd() == n) {
	deg++;
      }
    }
  }
  return deg;
}

template<typename T>
inline size_t AdjacencyList<T>::outDegree(const Node<T> *n) const {
  return _graph[n->getID()].size();
}

template<typename T>
inline const std::list<EdgePtr<T> > & AdjacencyList<T>::adjacent(const Node<T> *n) const {
  typename std::map<int, std::list<EdgePtr<T> > >::const_iterator iter = _graph.find(n->getID());
  if (iter == _graph.end()) {
    throw std::invalid_argument("Invalid Node ID - " + std::to_string(n->getID()) ); // if node doesn't exist
  }
  return iter->second;
}

template<typename T>
inline void AdjacencyList<T>::reset() {
  for_each (_nodes.begin(), _nodes.end(), ResetNode()); 
}

#endif // _ADJLIST_HH_
