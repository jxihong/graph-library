#ifndef _ADJMATRIX_HH_
#define _ADJMATRIX_HH_

#include <vector>
#include <list>
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
class AdjacencyMatrix;

/*
* Froward declaration of friend methods of the class
*/
template<typename T>
bool hasNegativeCycle(const AdjacencyMatrix<T> &g);

/*
 * Descrives the adjacency-matrix representation of the graph
 * data structure
 */
template <typename T>
class AdjacencyMatrix {

private:
  std::vector<std::vector<EdgePtr<T> > > _graph; // array that uses node ID as indices
  std::vector<Node<T>* > _nodes; // keeps track of all nodes
  
  bool _isDirected;
  
 
  struct DeleteObject  { 
    template<typename type> 
    void operator()(const type *ptr) const { 
      delete ptr; 
    } 
  };
  
  // Function object that resets state and weight of a node
  struct ResetNode : public std::unary_function<void, void> {
    void operator() (Node<T> *n) {
      n->setState(NOT_VISITED);
      n->setWeight(Node<T>::INFINITY);
    }
  };

  AdjacencyMatrix() = delete; // Removes default constructor

public:
  // Constructs graph from file input
  AdjacencyMatrix(std::istream &input, bool directed);
  ~AdjacencyMatrix();


  inline size_t size() { return _nodes.size(); }

  inline Node<T> *node(int id) const { return _nodes[id]; }
  
  inline bool isDirected() const { return _isDirected; }

  // Connects two existing nodes
  void addEdge(int from, int to, T weight);
  void addNode(int id);

  void removeEdge(int from, int to);
  
  // Computes number of incoming and outgoing edges
  size_t inDegree(const Node<T> *n) const;
  size_t outDegree(const Node<T> *n) const;

  // Returns a list of the non-null edges adjacent to specified node
  const std::list<EdgePtr<T> > adjacent(const Node<T> *n) const;
  
  // Sets all nodes as not visited with weight INFINITY
  void reset();

  friend std::ostream & operator<<(std::ostream &os, const AdjacencyMatrix<T> &g) {
    for (size_t i = 0; i < g._graph.size(); i++) {
      os << i << ":";
      for (auto& edge : g._graph[i]) {
	if (edge != NULL) {
	  os << "("
	     << edge->getStart()->getID() << ", "
	     << edge->getEnd()->getID() << ", "
	     << edge->getWeight() << ")";
	}
      }
      os << std::endl;
    }
    return os;
  }
};

template<typename T>
AdjacencyMatrix<T>::AdjacencyMatrix(std::istream &input, bool directed) 
  : _nodes(), _isDirected(directed) {

  size_t nNodes, nEdges;
  input >> nNodes >> nEdges;
  
  _nodes.resize(nNodes, nullptr);
  
  _graph.resize(nNodes);
  for (auto& cols : _graph) {
    cols.resize(nNodes, NULL);
  }

  int from, to; // id's to the two nodes
  T weight;
  while (input >> from >> to >> weight) {
    addNode(from);
    addNode(to);
    
    addEdge(from, to, weight);
  }
}

template<typename T>
AdjacencyMatrix<T>::~AdjacencyMatrix() {
  for_each(_nodes.begin(), _nodes.end(), DeleteObject());
  _nodes.clear();

  _graph.clear();
}

template<typename T>
inline void AdjacencyMatrix<T>::addEdge(int from, int to, T weight) {
  Edge<T> *e = new Edge<T>(_nodes[from], _nodes[to], weight);
  _graph[from][to] = EdgePtr<T>(e);

  if (!_isDirected) {
    Edge<T> *e_rev = new Edge<T>(_nodes[to], _nodes[from], weight);
    _graph[to][from] = EdgePtr<T>(e_rev);
  }
}

template<typename T>
inline void AdjacencyMatrix<T>::addNode(int id) {
  assert (id < (int) _nodes.size());

  if (_nodes[id] == nullptr) {
    Node<T> *new_node = new Node<T>(id);
    _nodes[id] = new_node;
  }
}
    
template<typename T>
inline void AdjacencyMatrix<T>::removeEdge(int from, int to) {
  _graph[from][to] = NULL;
  if (!_isDirected) {
    _graph[to][from] = NULL;
  }
}

template<typename T>
inline size_t AdjacencyMatrix<T>::inDegree(const Node<T> *n) const {
  size_t deg = 0;
  for (size_t i = 0; i < _graph.size(); i++) {
    for (auto &edge : _graph[i]) {
      if (edge->getEnd() == n) {
	deg++;
      }
    }
  }
  return deg;
}


template<typename T>
inline size_t AdjacencyMatrix<T>::outDegree(const Node<T> *n) const{
  return _graph[n->getID()].size();
}

template<typename T>
inline const std::list<EdgePtr<T> > AdjacencyMatrix<T>::adjacent(const Node<T> *n) const {
  std::list<EdgePtr<T> > edges;
  if (n->getID() < 0 || (size_t)n->getID() > _graph.size()) {
    throw std::invalid_argument("Invalid Node ID - " + std::to_string(n->getID()) ); // if node doesn't exist
  }
  
  copy_if(_graph[n->getID()].begin(), _graph[n->getID()].end(), std::inserter(edges, edges.begin()), 
	  [](EdgePtr<T> e){return !(e == NULL); } );
  
  return edges;
}
  
template<typename T>
inline void AdjacencyMatrix<T>::reset() {
  for_each (_nodes.begin(), _nodes.end(), ResetNode());
}
 
#endif //_ADJMATRIX_HH_
