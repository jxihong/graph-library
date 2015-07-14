#include "../include/AdjacencyList.hh"
#include "shortest_path.cpp"

#include <iomanip>

using namespace std;

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
  cout << my_graph << endl;
  try {
    cout << "Minimum Distance from Node 0 to 5 (Bellman-Ford): "
	 << BellmanFord<double>(my_graph, my_graph.node(0), my_graph.node(5), true)
	 << endl;
  }
  catch (runtime_error &e) {
    cerr << e.what() << endl;
      return -1;
  }
  return 0;
}

