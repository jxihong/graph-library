#include "../include/AdjacencyList.hh"

namespace graph {

  /*
   * Preforms recursive depth-first search starting from a specified node
   */
  template <typename T, class Graph_T>
  void DFS(Graph_T &g, Node<T> *src) {
    g.reset();
    
    src->setState(VISITED);
    
    for (auto& edge : g.adjacent(src)) {
      if (edge->getEnd()->getState() == NOT_VISITED) {
	DFS(g, edge->getEnd());
      }
    }
  }
  
  /*
   * Preforms non-recursive depth-first search starting from specified node
   */
  template <typename T, class Graph_T>
  void DFS_iterative(Graph_T &g, Node<T> *src, bool print = false) {
    g.reset();
    
    std::stack<Node<T>* > nodeStack;
    std::vector<int> order; // allows you to print the order of traversal
    
    nodeStack.push(src);
    
    while (!nodeStack.empty()) {
    auto top = nodeStack.top();
    nodeStack.pop();
    
    if (top->getState() == NOT_VISITED) {
      top->setState(VISITED);
      order.push_back(top->getID());
    }
    
    for (auto& edge : g.adjacent(top)) {
      nodeStack.push(edge->getEnd());
    }
    
    }
    
    if (print) {
      copy(order.begin(), order.end()-1, std::ostream_iterator<int>(std::cout, ", "));
      std::cout << order.back(); // prints last element without the delimiter
    }
  }
  
  /*
   * Preforms breadth-first search on a specified start node
   */
  template<typename T, class Graph_T>
  void BFS(Graph_T &g, Node<T> *src, bool print = false) {
    g.reset();
  
    std::queue<Node<T>* > nodeQueue;
    std::vector<int> order;
    
    nodeQueue.push(src);
    
    while (!nodeQueue.empty()) {
      auto front = nodeQueue.front();
      nodeQueue.pop();
      
      front->setState(VISITED);
      order.push_back(front->getID());
      for (auto& edge : g.adjacent(front)) {
	if (edge->getEnd()->getState() == NOT_VISITED) {
	  edge->getEnd()->setState(PENDING);
	  nodeQueue.push(edge->getEnd());
	}
      }
    }
    
    if (print) {
      copy(order.begin(), order.end()-1, std::ostream_iterator<int>(std::cout, ", "));
      std::cout << order.back(); // prints last element without the delimiter
    }
  }
}
