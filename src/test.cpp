#include "shortest_path.cpp"
#include "search.cpp"
#include "sort.cpp"

#include <iomanip>

using namespace std;
using namespace graph;

/*
 * Only tests the Adjacency List representation, but testing the Adjacency Matrix representation
 * should theoretically be identical
 */
int main(int argc, char *argv[]) {
 
  ifstream f;
  bool hasFile = false;

  string usage = "Usage: " + string(argv[0]) + " -f <filename>";

  if (argc > 1) {
    if (string(argv[1]) != "-f") {
      cerr << usage << endl;
      return -1;
    }
    
    f.open(argv[2]);
    if (!f.good()) {
      cerr << "Couldn't open file " << argv[2] << endl;
      cerr << usage << endl;
      return -1;
    }
    
    hasFile = true;
  }

  istream &in = hasFile ? f : cin;
  
  auto my_graph = AdjacencyList<double>(in, true);
  cout << fixed << setprecision(1);
  cout << "Graph from File:" << endl;
  cout << my_graph << endl;
  
  (!hasCycle<double>(my_graph)) ? cout << "Graph is cyclic. " << endl : 
    cout << "Graph contains no cycles. " << endl;
  
  cout << endl;

  try {
    cout << "Graph Algorithms." << endl;

    cout << "1. Depth-first Search from Node 0: ";
    DFS_iterative(my_graph, my_graph.node(0), true);
    cout << endl;
    
    cout << "2. Breadth-first Search from Node 0: ";
    BFS(my_graph, my_graph.node(0), true);
    cout << endl;

    cout << endl;

    cout << "3. Minimum Distance from Node 0 to 5 (Bellman-Ford): "
	 << BellmanFord(my_graph, my_graph.node(0), my_graph.node(5), true)
	 << endl;

    cout << endl;

    cout << "4. All-Pairs Shortest Path:" << endl;
    
    vector<vector<double> > distances; 
    FloydWarshall(my_graph, distances);  
    for (size_t i = 0; i < my_graph.size(); i++) {
      cout << i << ": ";
      for (size_t j = 0; j < my_graph.size(); j++) {
	if (distances[i][j] == numeric_limits<double>::max()) {
	  cout << j << "|NA, ";
	}
	else {
	  cout << j << "|" << distances[i][j] << ", ";
	}
      }
      cout << endl;
    }

  }
  catch (const exception &e) {
    cerr << e.what() << endl;
      return -1;
  }
}
