#include "glotter.hpp"
using namespace std;

Glotter glotter;

template <typename T> T load() { T r; cin >> r; return r; }
struct Graph {
    vector<vector<int>> edges;
    Graph(int n):edges(n){glotter.resize(n);}
    void addEdge2(int a, int b){
        edges[a].push_back(b);
        edges[b].push_back(a);
        glotter.addEdge2(a,b);
    }

    template <typename F> void BFS(int vertex, F f) {
        std::vector<int> as = {{vertex}}, bs;
        std::vector<bool> visit(size(), false);
        visit[vertex] = true;
        while (not as.empty()) {
            for (auto v : as) glotter.setVertexColor(v, "#ff0000");
            for (auto v : as) {
                f(v);
                for (auto kid : edges[v]) {
                    glotter.setEdge1Color(v, kid, "#888888");
                    if (!visit[kid]) {
                        visit[kid] = true;
                        bs.push_back(kid);
                        glotter.setVertexColor(kid, "#888800");
                    }
                }
            }
            as = std::move(bs);
        }
    }
    int size() { return edges.size(); }
};

int main() {
    auto n = load<int>();
    auto m = load<int>();
    auto graph = Graph(n);
    for (int i=0; i<m; ++i) {
        auto a = load<int>() - 1;
        auto b = load<int>() - 1;
        graph.addEdge2(a, b);
    }
    graph.BFS(0, [&](int){});
}