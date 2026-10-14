#include <iostream>
#include <vector>
#include "Graph.h"

using namespace std;

int main() {
    vector<vector<int> > matrix = {
        {0,  2,  1,  7,  0,  0,  0,  0,  0},
        {2,  0,  5,  5,  0,  0,  0,  0,  0},
        {1,  5,  0,  4,  0,  9,  0,  0,  0},
        {7,  5,  4,  0,  8,  0,  0,  0,  0},
        {0,  0,  0,  8,  0,  3,  7,  0,  0},
        {0,  0,  9,  0,  3,  0,  5, 10,  0},
        {0,  0,  0,  0,  7,  5,  0, 11,  6},
        {0,  0,  0,  0,  0, 10, 11,  0,  3},
        {0,  0,  0,  0,  0,  0,  6,  3,  0}
    };

    Graph g(matrix);

    cout << "Part 1: graph properties \n";
    g.report();

    cout << endl << "Part 2: Dijkstra from A \n";
    g.printShortestPaths("A");

    cout << endl << "Test: Dijkstra from H \n";
    g.printShortestPaths("H");

    cout << boolalpha;

    cout << endl << "Test: parallel edge + loop \n";
    Graph g2(matrix);
    g2.addEdge("A", "B", 9);
    g2.addEdge("C", "C", 1);
    cout << "Multigraph  : " << g2.isMultigraph()  << "\n";
    cout << "Pseudograph : " << g2.isPseudograph() << "\n";

    cout << endl << "Test: directed graph \n";
    vector<vector<int> > m3 = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 0}
    };
    Graph g3(m3);
    cout << "Digraph     : " << g3.isDigraph() << "\n";
    g3.printShortestPaths("B");

    cout << endl << "Test: complete graph K3 \n";
    vector<vector<int> > m4 = {
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}
    };
    Graph g4(m4);
    cout << "Complete    : " << g4.isComplete() << "\n";
    cout << "Weighted    : " << g4.isWeighted() << "\n";

    cout << endl << "Test: isolated node + unreachable \n";
    vector<vector<int> > m5 = {
        {0, 2, 0},
        {2, 0, 0},
        {0, 0, 0}
    };
    Graph g5(m5);
    cout << "Disjointed  : " << g5.isDisjointed() << "\n";
    g5.printShortestPaths("A");

    return 0;
}