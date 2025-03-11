#include "dijkstras.h"
#include <iostream>
#include <string>
#include <exception>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <graph filename>" << endl;
        return 1;
    }

    Graph G;
    try {
        file_to_graph(argv[1], G);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    int source = 0;

    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, source, previous);

    cout << "\nDijkstra's shortest paths from vertex " << source << ":\n";
    for (int i = 0; i < G.numVertices; ++i) {
        cout << i;
        vector<int> path = extract_shortest_path(distances, previous, i);
        if (path.empty()) {
            cout << "No path." << endl;
        } else {
            print_path(path, distances[i]);
        }
    }

    return 0;
}
