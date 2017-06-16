#include <bits/stdc++.h>
using namespace std;

template <typename T> T load() { T r; cin >> r; return r; }

struct Tree {
	vector<vector<int>> edges;
	Tree(int n):edges(n){
	}
	void addEdge1(int a, int b) {
		edges[a].push_back(b);
	}
	void addEdge2(int a, int b) {
		addEdge1(a, b);
		addEdge1(b, a);
	}
	int size() { return edges.size(); }
	int sizelog2() {
		int r = 0;
		while ((1 << (r+1)) <= size())
			++r;
		return r;
	}

	template <typename Pre, typename Post> void DFS(int source, Pre pre, Post post) {
		auto visit = vector<bool>(size(), false);
		DFSImpl(source, pre, post, visit);
	}
	template <typename Pre, typename Post> void DFSImpl(int vertex, Pre pre, Post post, vector<bool>& visit) {
		if (!visit[vertex]) {
			visit[vertex] = true;
			pre(vertex);
			for (auto kid : edges[vertex])
				DFSImpl(kid, pre, post, visit);
			post(vertex);
		}
	}

	class MachineLCA {
		Tree* tree;
		vector<vector<int>> jump;
		vector<int> height;
		friend Tree;
	public:

		int lca(int a, int b) {
			tie(a, b) = equalHeight(a, b);
			for (int i=tree->sizelog2()-1; i>=0; --i) {
				if (jump[a][i] != jump[b][i]) {
					tie(a, b) = make_pair(jump[a][i], jump[b][i]);
				}
			}
			return jump[a][0];
		}

	private:
		pair<int, int> equalHeight(int a, int b) {
			if (height[a] > height[b])
				swap(a, b);
			return make_pair(a, downTo(b, height[a]));
		}
		int downTo(int v, int h) {
			for (int i=tree->sizelog2()-1; i>=0; --i) {
				if (height[jump[v][i]] >= h) {
					v = jump[v][i];
				}
			}
			return v;
		}
	};
	MachineLCA prepareLCAPow2Jump(int root) {
		auto mach = MachineLCA();
		auto lg = sizelog2();
		mach.tree = this;
		mach.jump.resize(size(), vector<int>(lg, -1));
		mach.height.resize(size(), -1);

		for (int i=0; i<lg; ++i)
			mach.jump[root][i] = root;
		mach.height[root] = -1;

		auto stackDFS = stack<int>();
		this->DFS(root, [&](int vertex) {
			auto parent = stackDFS.empty() ? root : stackDFS.top();
			stackDFS.push(vertex);

			mach.jump[vertex][0] = parent;
			mach.height[vertex] = mach.height[parent] + 1;
			for (int i=1; i<lg; ++i) {
				auto half = mach.jump[vertex][i-1];
				auto full = mach.jump[half][i-1];
				mach.jump[vertex][i] = full;
			}
		}, [&](int){
			stackDFS.pop();
		});

		return mach;
	}
};

Tree loadTree(int n) {
	auto tree = Tree(n);
	for (int i=0; i<n-1; ++i) {
		auto a = load<int>()-1;
		auto b = load<int>()-1;
		tree.addEdge2(a, b);
	}
	return tree;
}

int main() {
	auto n = load<int>();
	auto tree = loadTree(n);
	auto mach = tree.prepareLCAPow2Jump(0);
	auto a = load<int>()-1;
	auto b = load<int>()-1;
	auto r = mach.lca(a, b);
	cout << (r+1) << endl;
}