#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// 返回图中从 s 到某节点的路径, s 为起始节点的索引
template <typename Graph>
class Path {
private:
    Graph &G;
    int s;
    bool *visited; // 记录节点是否已经被访问
    int *from; // 记录访问到当前节点的上一个节点是哪个, 用于最后恢复处完整的遍历路径

    void dfs(int v) {
        visited[v] = true;
        typename Graph::adjIterator adj(G, v);
        for (int w = adj.begin(); !adj.end(); w = adj.next()) {
            if (!visited[w]) {
                from[w] = v; // 说明是从 v 节点后再访问的 w 节点
                dfs(w);
            }
        }
    }

public:
    Path(Graph &graph, int s) : G(graph) {
        // 算法初始化
        assert( s >= 0 && G.V() );
        visited = new bool[G.V()];
        from = new int[G.V()];
        for (int i = 0; i < G.V(); ++i) {
            visited[i] = false;
            from[i] = -1;
        }
        this->s = s;

        // 寻路算法
        dfs(s);
    }

    ~Path() {
        delete[] visited;
        delete[] from;
    }

    // s 到 w 之间是否存在路径, 如果存在, 那么 w 必然被访问过.
    bool hasPath(int w) {
        assert( w >= 0 && w < G.V() );
        return visited[w];
    }

    // 从 from 中倒序得到从 s 到 w 的完整路径, 由于是倒着得到的,
    // 所以需要使用栈
    void path(int w, vector<int> &vec) {
        stack<int> stk;
        int p = w;
        // 由于源节点 s 的 from[s] = -1, 当访问源节点后, p = -1,
        // 跳出循环. 因此最后源节点 s 是被加到栈中的.
        while (p != -1) {
            stk.push(p);
            p = from[p];
        }

        vec.clear(); // 防止 vec 不是空的
        while (!stk.empty()) {
            vec.push_back(stk.top());
            stk.pop();
        }
    }

    void showPath(int w) {
        vector<int> vec;
        path(w, vec);

        cout << "DFS PATH: ";
        for (int i = 0; i < vec.size(); ++i) {
            cout << vec[i];
            if (i == vec.size() - 1)
                cout << endl;
            else
                cout << " -> ";
        }
    }
};

#endif
