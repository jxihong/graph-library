#ifndef _EDGE_HH_
#define _EDGE_HH_

#include <memory>

template <typename T> class Node;

/*
 * Describes an edge between two vertices, with weight
 */
template <typename T>
class Edge {

private:
  Node<T> *_start;
  Node<T> *_end;

  T _weight;

  Edge() = delete; // Removes the default constructor

public:
  Edge(Node<T> *s, Node<T> *e, T w = (T)0) : _start(s), _end(e), _weight(w) {}
  
  ~Edge() {}

  inline Node<T>* getStart() const { return _start; }
  inline Node<T>* getEnd() const { return _end; }
  
  inline T getWeight() const { return _weight; }
};

template<typename T>
using EdgePtr = std::shared_ptr<Edge<T> >;

#endif // _EDGE_HH_

  

