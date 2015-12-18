#include "../include/AdjacencyList.hh"
#include "../include/AdjacencyMatrix.hh"

namespace graph {
  /*
   * Finds a topological sort of the graph
   */
  template <typename T, class Graph_T>
  std::vector<Node<T>* > TopologicalSort(Graph_T &g) {
    if (!g.isDirected()) { throw std::invalid_argument("Graph must be directed."); }

    g.reset();
    
    std::map<int, size_t> nodeDegrees;
    
    std::queue<Node<T>* > nodeQ; // Nodes with no incoming edges
    std::set<EdgePtr<T> > edges; // Checks visited edges
    std::vector<Node<T>* > order; // Topological sorting
    
    Node<T>* nodePtr;
    // Preprocessing: Compute incoming degree of all nodes
    for (size_t i = 0; i < g.size(); i++) {
      nodePtr = g.node(i);

      nodeQ.push(nodePtr);
      nodeDegrees[nodePtr->getID()] = g.inDegree(nodePtr);      
      if (g.inDegree(nodePtr) == 0) { nodeQ.push(nodePtr); }
    }
    
    while (!nodeQ.empty()) {
      nodePtr = nodeQ.front();
      nodeQ.pop();
      
      order.push_back(nodePtr);
      for (auto& edge: g.adjacent(nodePtr)) {
	// Checks for cycles in the graph
	auto insert_result = edges.insert(edge);
	if (!insert_result.second) { throw std::runtime_error("Graph contains cycle."); }
	
	Node<T> *neighbor = edge->getEnd();
	nodeDegrees[neighbor->getID()]--;
	if (nodeDegrees[neighbor->getID()] == 0) {
	  nodeQ.push(neighbor);
	}
      } 
    }
	
    return order;
  }

  /*
   * Determines if there is a cycle in the graph
   */
  template <typename T, class Graph_T>
  bool hasCycle(Graph_T &g) {
    try {
      std::vector<Node<T>* > toposorted = TopologicalSort<T>(g);
    } 
    catch (std::runtime_error &e) {
      return true;
    }
    return false;
  }
}
