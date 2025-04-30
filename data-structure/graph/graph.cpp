#include <iostream>
#include <vector>

using namespace std;

class Graph {
private:
    vector<vector<int>> adj;
    int n;

public:
    Graph(int n) {
        this->n = n;
        adj = vector<vector<int>>(n, vector<int>(n, 0));
    }

    void insert_edge(int u, int v) {
        adj[u][v] = 1;
        adj[v][u] = 1;
    }

    void remove_edge(int u, int v) {
        adj[u][v] = 0;
        adj[v][u] = 0;
    }

    bool has_edge(int u, int v) const {
        return adj[u][v];
    }

    int number_edges(int u) const {
        int count = 0;

        for(int v = 0; v < n; v++) 
            if(adj[u][v])
                count++;

        return count;
    }

    void print_edges() const {
        for(int u = 0; u < n; u++)
            for(int v = u + 1; v < n; v++)
                if(adj[u][v])
                    cout << "{" << u << ", " << v << "}\n";
    }

    int degree(int u) const {
        int degree = 0;
        for(int v = 0; v < n; v++)
            if(adj[u][v])
                degree++;

        return degree;
    }

    int most_popular() const {
        int max_vertice = 0;
        int max_degree = degree(0);
        for(int u = 1; u < n; u++) {
            int actual_degree = degree(u);
            if(actual_degree > max_degree) {
                max_degree = actual_degree;
                max_vertice = u;
            }
        }

        return max_vertice;
    }

    void print_recommendations(int u) const {
        for(int v = 0; v < n; v++) 
            if(adj[u][v])
                for(int w = 0; w < n; w++) 
                    if(adj[v][w] && w != u && !adj[u][w])
                        cout << w << endl;
    }

    bool is_complete() const {
        bool complete = true;
        for(int u = 0; u < n; u++) {
            for(int v = u + 1; v < n; v++) {
                if(!adj[u][v])
                    complete = false;
            }
        }

        return complete;
    }

    static Graph read_graph() {
        int n, m;
        cin >> n >> m;
        Graph g(n);
        for(int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            g.insert_edge(u, v);
        }

        return g;
    }
};

int main() {
    Graph g(5);

    g.insert_edge(0, 1);
    g.insert_edge(0, 2);
    g.insert_edge(1, 2);
    g.insert_edge(1, 3);
    g.insert_edge(3, 4);

    g.print_edges();
 
    cout << g.has_edge(0, 1) << endl;
    cout << g.has_edge(0, 4) << endl;

    g.remove_edge(1, 2);
    g.print_edges();

    cout << g.degree(1) << endl;
    cout << g.degree(3) << endl;

    cout << g.most_popular() << endl;

    g.print_recommendations(0);
    g.print_recommendations(3);

    cout << "Read a new graph from the keyboard (u v followed by v edges): ";
    Graph g2 = Graph::read_graph();
    g2.print_edges();

    Graph g3(4);
    g3.insert_edge(0, 1);
    g3.insert_edge(1, 2);
    g3.insert_edge(2, 3);
    g3.insert_edge(3, 0);
    cout << g3.is_complete() << endl;

    cout << g3.number_edges(0) << endl;
    cout << g3.number_edges(1) << endl;
    cout << g3.number_edges(2) << endl;
    cout << g3.number_edges(3) << endl;
}