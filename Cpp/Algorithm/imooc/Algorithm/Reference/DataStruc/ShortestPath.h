#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include <iostream>
#include <queue>
#include <cassert>
#include <stack>

using namespace std;

// 从广度优先遍历的结果可以发现, 预先遍历的节点与起始节点的路径总是
// 小于或等于后面遍历的节点与起始节点的距离.
template <typename Graph>
class ShortestPath {
private:
    Graph &G; // 保存图
    int s; // 起始节点的索引
    bool *visited;
    int *from;
    int *ord; // 距离起始节点的最短距离, order
public:
    ShortestPath(Graph &graph, int s) : G(graph) {
        // 算法初始化
        assert( s >= 0 && s < G.V() );
        visited = new bool[G.V()];
        from = new int[G.V()];
        ord = new int[G.V()];
        for (int i = 0; i < G.V(); ++i) {
            visited[i] = false;
            from[i] = -1;
            ord[i] = -1;
        }
        this->s = s;
        queue<int> q; // 队列, 用于对图进行广度优先遍历

        // 无向图最短路径算法
        q.push(s);
        visited[s] = true; // 推入 s 到队列后, 就说明已经访问过了
        ord[s] = 0; // 自己到自己的距离为 0

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            typename Graph::adjIterator iter(G, v);
            for (int w = iter.begin(); !iter.end(); w = iter.next()) {
                if (!visited[w]) { // 如果 w 没有被访问过, 说明 w 不在队列中
                    q.push(w);
                    visited[w] = true; // 此时 w 加入到队列中了
                    from[w] = v; // 记录访问到它的路径上前一个节点
                    ord[w] = ord[v] + 1; // 更新 w 距离起始节点的距离, 为它前一个节点的距离加上 1.
                }
            }
        }
    }

    ~ShortestPath() {
        delete[] visited;
        delete[] from;
        delete[] ord;
    }

    int length(int w) {
        assert( w >= 0 && w < G.V() );
        return ord[w];
    }

    // s 到 w 之间是否存在路径, 如果存在, 那么 w 必然被访问过.
    bool hasPath(int w) {
        assert( w >= 0 && w < G.V() );
        return visited[w];
    }

    // 从 from 中倒序得到从 s 到 w 的完整路径, 由于是倒着得到的,
    // 所以需要使用栈
    void path(int w, vector<int> &vec) {
        assert( w >= 0 && w < G.V() );
        stack<int> stk;
        int p = w;
        while (p != -1) {
            stk.push(p);
            p = from[p];
        }

        vec.clear();
        while (!stk.empty()) {
            vec.push_back(stk.top());
            stk.pop();
        }
    }

    void showPath(int w) {
        vector<int> vec;
        path(w, vec);

        cout << "BFS PATH: ";
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
