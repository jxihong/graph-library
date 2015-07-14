#ifndef _NODE_HH_
#define _NODE_HH_

#include <limits>

enum STATUS { VISITED, PENDING, NOT_VISITED }; // Defines possible states of node

/*
 * Describes a single vertex on a graph, with ID and weight
 */
template <typename T>
class Node {
  
private:
  int _id; 
  T _weight; // Optional parameter for weight of node

  STATUS _state; // Specifies the status of a node

  Node() = delete; // Removes the default constructor

public:
  static constexpr const T INFINITY = std::numeric_limits<T>::max();
 
  Node(int id) : _id(id),  _weight(INFINITY), _state(NOT_VISITED) {}
  ~Node() {}
  
  inline int getID() const { return _id; }

  inline T getWeight() const { return _weight; }
  inline void setWeight(T w) { _weight = w; } 
  
  inline STATUS getState() const { return _state; }
  inline void setState(STATUS s) { _state = s; }
};

#endif // _NODE_HH_
