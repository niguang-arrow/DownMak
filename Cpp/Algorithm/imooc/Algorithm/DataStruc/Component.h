// 求一个图的联通分量, 使用图的深度优先遍历

#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <cassert>

using namespace std;


// Component 不仅能计算出联通分量的个数(使用图的深度优先遍历),
// 还可以判断图中的任意两个节点是否连接(使用的是并查集中的 id, 
// 在同一联通分量中的节点的id 设置为相同的)
template <typename Graph>
class Component {
private:
    Graph &G; // 存储一份 graph
    bool *visited; //记录每个节点是否访问过
    int ccount; // 联通分量的数量
    int *id; // 使用并查集, 判断两个节点是否相连, 相连节点的 id 相同

    // 深度优先遍历
    // 对 v 进行 dfs, 首先说明 v 被遍历到了, 所以设置 visited 为 true,
    // 之后依次访问与 v 相连的所有节点, 分别进行 dfs.
    void dfs(int v) {
        visited[v] = true;
        id[v] = ccount; // 不同联通分量中的节点的 id 不同, 而相同联通分量中的节点的id相同

        // 注意这里要使用 typename
        // 访问 v 的所有邻边
        typename Graph::adjIterator adj(G, v);
        for (int i = adj.begin(); !adj.end(); i = adj.next()) {
            if (!visited[i])
                dfs(i);
        }
    }

public:
    Component(Graph &graph) : G(graph) {
        visited = new bool[G.V()];
        id = new int[G.V()];
        ccount = 0;
        for (int i = 0; i < G.V(); ++i) {
            visited[i] = false; // 初始表示所有节点都没有被访问
            id[i] = -1; //初始化 id 为 -1;
        }

        // 求联通分量, 使用深度优先遍历, 对于每一个节点, 如果没有被访问到,
        // 那么就使用深度优先去访问, 然后统计出联通分量的大小,
        // 注意没有遍历的节点一定在另外的联通分量中.
        for (int i = 0; i < G.V(); ++i) {
            if (!visited[i]) {
                dfs(i);
                ccount ++;
            }
        }
    }

    ~Component() {
        delete[] visited;
        delete[] id;
    }
    int count() const { return ccount; } // 返回联通分量

    bool isConnected(int v, int w) {
        assert( v >= 0 && v < G.V() );
        assert( w >= 0 && w < G.V() );
        return id[v] == id[w];
    }
};

#endif
