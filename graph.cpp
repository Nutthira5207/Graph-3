#include <iostream>
#include <set>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include "Graph.h"

using namespace std;

int Graph::indexOf(const string& name) const {
    for (size_t i = 0; i < nodes.size(); i++)
        if (nodes[i].name == name) return (int)i;
    throw invalid_argument("Node not found: " + name);
}

Graph::Graph(const vector<vector<int> >& matrix) : directed(false) {
    int n = matrix.size();

    for (int i = 0; i < n; i++)
        if ((int)matrix[i].size() != n)
            throw invalid_argument("Adjacency matrix must be square (n x n)");

    for (int i = 0; i < n; i++)
        nodes.push_back(Node(string(1, (char)('A' + i))));

    for (int i = 0; i < n && !directed; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j] != matrix[j][i]) {
                directed = true;
                break;
            }

    for (int i = 0; i < n; i++) {
        int start = directed ? 0 : i;
        for (int j = start; j < n; j++) {
            if (matrix[i][j] != 0)
                edges.push_back(Edge(i, j, matrix[i][j], directed));
        }
    }
}

void Graph::addEdge(const string& src, const string& dst, int weight) {
    edges.push_back(Edge(indexOf(src), indexOf(dst), weight, directed));
}

bool Graph::isMultigraph() const {
    set<pair<int, int> > seen;
    for (size_t k = 0; k < edges.size(); k++) {
        pair<int, int> key = edges[k].key();
        if (seen.count(key)) return true;
        seen.insert(key);
    }
    return false;
}

bool Graph::isPseudograph() const {
    for (size_t k = 0; k < edges.size(); k++)
        if (edges[k].source == edges[k].target) return true;
    return false;
}

bool Graph::isDigraph() const {
    return directed;
}

bool Graph::isWeighted() const {
    for (size_t k = 0; k < edges.size(); k++)
        if (edges[k].weight != 1) return true;
    return false;
}

bool Graph::isComplete() const {
    set<pair<int, int> > pairs;
    for (size_t k = 0; k < edges.size(); k++) {
        pairs.insert(make_pair(edges[k].source, edges[k].target));
        if (!directed)
            pairs.insert(make_pair(edges[k].target, edges[k].source));
    }
    int n = nodes.size();
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++)
            if (u != v && !pairs.count(make_pair(u, v)))
                return false;
    return true;
}

vector<string> Graph::isolatedNodes() const {
    vector<bool> connected(nodes.size(), false);
    for (size_t k = 0; k < edges.size(); k++) {
        if (edges[k].source != edges[k].target) {
            connected[edges[k].source] = true;
            connected[edges[k].target] = true;
        }
    }
    vector<string> result;
    for (size_t i = 0; i < nodes.size(); i++)
        if (!connected[i]) result.push_back(nodes[i].name);
    return result;
}

bool Graph::isDisjointed() const {
    return !isolatedNodes().empty();
}

void Graph::report() const {
    cout << boolalpha;

    cout << "Nodes (" << nodes.size() << "): ";
    for (size_t i = 0; i < nodes.size(); i++)
        cout << nodes[i].name << " ";
    cout << "\n";

    cout << "Edges (" << edges.size() << "):\n";
    for (size_t k = 0; k < edges.size(); k++) {
        const Edge& e = edges[k];
        cout << "  " << nodes[e.source].name
             << (e.directed ? " -> " : " -- ")
             << nodes[e.target].name
             << "  (w=" << e.weight << ")\n";
    }

    cout << endl << "Multigraph  : " << isMultigraph()  << "\n";
    cout << "Pseudograph : " << isPseudograph() << "\n";
    cout << "Digraph     : " << isDigraph()     << "\n";
    cout << "Weighted    : " << isWeighted()    << "\n";
    cout << "Complete    : " << isComplete()    << "\n";
    cout << "Disjointed  : " << isDisjointed()  << "  (isolated: ";
    vector<string> iso = isolatedNodes();
    for (size_t i = 0; i < iso.size(); i++) cout << iso[i] << " ";
    cout << ")\n";
}

vector<int> Graph::dijkstra(const string& startName, vector<int>& prev) const {
    int n = nodes.size();
    int s = indexOf(startName);

    vector<vector<pair<int, int> > > adj(n);
    for (size_t k = 0; k < edges.size(); k++) {
        const Edge& e = edges[k];
        if (e.weight < 0)
            throw invalid_argument("Dijkstra does not support negative weights");
        adj[e.source].push_back(make_pair(e.target, e.weight));
        if (!e.directed)
            adj[e.target].push_back(make_pair(e.source, e.weight));
    }

    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    prev.assign(n, -1);
    dist[s] = 0;

    typedef pair<int, int> P;
    priority_queue<P, vector<P>, greater<P> > pq;
    pq.push(make_pair(0, s));

    while (!pq.empty()) {
        P top = pq.top();
        pq.pop();
        int u = top.second;

        if (visited[u]) continue;
        visited[u] = true;

        for (size_t k = 0; k < adj[u].size(); k++) {
            int v = adj[u][k].first;
            int w = adj[u][k].second;
            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return dist;
}

string Graph::pathTo(int target, const vector<int>& prev) const {
    vector<int> path;
    for (int v = target; v != -1; v = prev[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    string result;
    for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) result += " -> ";
        result += nodes[path[i]].name;
    }
    return result;
}

void Graph::printShortestPaths(const string& startName) const {
    vector<int> prev;
    vector<int> dist = dijkstra(startName, prev);

    cout << "Shortest paths from " << startName << ":\n";
    for (size_t i = 0; i < nodes.size(); i++) {
        cout << "  to " << nodes[i].name << " : ";
        if (dist[i] == INF)
            cout << "unreachable\n";
        else
            cout << "distance = " << dist[i]
                 << ",  path: " << pathTo((int)i, prev) << "\n";
    }
}