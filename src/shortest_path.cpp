#include "../include/AdjacencyList.hh"
#include "../include/AdjacencyMatrix.hh"


// Forward declaration of helper methods
template<typename T>
void printPath (Node<T> *src, Node<T> *dest, const std::vector<Node<T>* > &prev);

template<typename T>
inline bool operator<(const Node<T> *a, const Node<T> *b) {
  return (a->getWeight() < b->getWeight());
}

namespace graph {
  
  /*
   * Finds the shortest path between two nodes, given that all edge weights
   * are positive
   */
  template<typename T, class Graph_T>
  T Dijkstra(Graph_T &g, Node<T> *src, Node<T> *dest, bool print = false) {
    g.reset(); // Sets all nodes to unvisited and weight INFINITY
    
    std::priority_queue<Node<T>* > minDist;
    std::vector<Node<T>* > prevNode(g.size(), nullptr); // allows you to print order of traversal
    
    src->setWeight((T) 0); // sets starting point with weight 0
    minDist.push(src); // adds starting point to the heap
    prevNode[src->getID()] = src;
    
    while (!minDist.empty()) {
      auto top = minDist.top();
      minDist.pop();
      if (top->getState() == NOT_VISITED) {
	
	top->setState(VISITED);
	
	for (auto& edge : g.adjacent(top)) {
	  if (edge->getWeight() < 0) { // throw exception if edge weights are positive
	    throw std::runtime_error("Error: Negative Edge Weight - " + 
				     std::to_string(edge->getWeight()));
	  }
	  Node<T> *neighbor = edge->getEnd();
	  if (neighbor->getWeight() >= top->getWeight() + edge->getWeight()){
	    neighbor->setWeight(top->getWeight() + edge->getWeight());
	    prevNode[neighbor->getID()] = top;
	  }
	  minDist.push(neighbor);
	}
      }
    }
    
    if (print) { // prints shortest path
      printPath(src, dest, prevNode);
      std::cout << " => ";
    }
    
  return dest->getWeight();
  }
  
  /*
   * Finds the shortest path between two nodes.
   */
  template<typename T, class Graph_T>
  T BellmanFord(Graph_T &g, Node<T> *src, Node<T> *dest, bool print = false) {
    g.reset(); // Sets all nodes to unvisited and weight INFINITY
    
    std::vector<Node<T>* > prevNode(g.size(), nullptr); // allows you to print order of traversal
    
    src->setWeight((T) 0);
    prevNode[src->getID()] = src;
    
    for (size_t i = 0; i < g.size(); i++) {
      for (auto& edge : g.adjacent(g.node(i))) {
	if (edge->getStart()->getWeight() + edge->getWeight() <=
	    edge->getEnd()->getWeight()) {
	  
	  edge->getEnd()->setWeight(edge->getStart()->getWeight() + edge->getWeight());
	  prevNode[edge->getEnd()->getID()] = edge->getStart();
	}
      }
    }
    
    // Checks for Negative Cycle
    for (size_t i = 0; i < g.size(); i++) {
      for (auto& edge : g.adjacent(g.node(i))) {
	if (edge->getStart()->getWeight() + edge->getWeight() <
	    edge->getEnd()->getWeight()) {
	  
	   throw std::runtime_error("Graph contains negative-weight cycle");
	}
      }
    }

    if (print) {
      printPath(src ,dest, prevNode);
      std::cout << " => ";
    }
    
    return dest->getWeight();
  }
  
  
  /*
   * Finds the all-pairs shortest path
   */
  template<typename T, class Graph_T>
  void FloydWarshall(Graph_T &g, std::vector<std::vector<T> > &dist) {
    g.reset();
    
    // Vector reference gets passed in from outside method
    dist.resize(g.size(), std::vector<double>(g.size(), std::numeric_limits<T>::max()));
    
    for (size_t i = 0; i < g.size(); i++) {
      dist[i][i] = 0; 
      // Initialize distances for all edges
      for (auto &edge : g.adjacent(g.node(i))) {
	dist[i][edge->getEnd()->getID()] = edge->getWeight();
      }
    }
    
    for (size_t k = 0; k < g.size(); k++) { 
      for (size_t i = 0; i < g.size(); i++) { 
	for (size_t j = 0; j < g.size(); j++) { 
	  if (dist[i][j] > dist[i][k] + dist[k][j]) {
	    dist[i][j] =  dist[i][k] + dist[k][j];
	  }
	}
      }
    }

    // Checks for Negative Cycle
    for (size_t i = 0; i < g.size(); i++) {
      for (auto& edge : g.adjacent(g.node(i))) {
	if (edge->getStart()->getWeight() + edge->getWeight() <
	    edge->getEnd()->getWeight()) {
	  
	   throw std::runtime_error("Graph contains negative-weight cycle");
	}
      }
    }
    
  }
}


/*
 * Prints the path of node ID's given the source, destination, and a vector showing each node's
 * previous node in the order of traversal
 */
template<typename T>
void printPath (Node<T> *src, Node<T> *dest, const std::vector<Node<T>* > &prev) {
  Node<T>* nodePtr = dest;
  
  std::vector<int> path; // traces the path by following vector of previous nodes
  path.push_back(nodePtr->getID());
    
  while (nodePtr != src) {
    nodePtr = prev[nodePtr->getID()];
    path.push_back(nodePtr->getID());
  }
  
  reverse(path.begin(), path.end()); // reverses the path to start with source node
  
  std::cout << "(";
  // prints each node ID, followed by the delimiter '->'
  copy(path.begin(), path.end()-1, std:: ostream_iterator<int>(std::cout, " -> "));
  std::cout << path.back() << ")"; //prints last element without '->'
}  
