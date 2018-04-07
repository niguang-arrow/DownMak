#include <iostream>
#include <vector>
#include <cassert>
#include "Graph.h" // 稀疏图和稠密图的定义
#include "ReadGraph.h" // 用于从文件中读取图
#include "Component.h" // 用于求联通分量(dfs)
#include "Path.h" // 用于求两个节点之间的路径 (dfs)
#include "ShortestPath.h" // 用于求两个节点之间的最短路径(BFS, 广度优先遍历)


using namespace std;

// 深度优先, 就是从一个点一直向下试, 直到试不下去为止.
// 和树不同的是, 图可能存在环, 所以需要记录已经遍历过的节点.
// 
// 广度优先遍历(BFS), 就是一次将图中一个节点的所有相邻节点全都遍历,
// 之后再去遍历这些节点的相邻节点.
// 广度优先遍历求出了无权图的最短路径, 注意是无权图.
// 
// 图的深度优先遍历和广度优先遍历的复杂度是一样的:
// 稀疏图(邻接表) : O(V + E)
// 稠密图(邻接矩阵) : O(V^2)
int main() {
    string filename = "testG1.txt";

    SparseGraph g1(13, false); // 13 个节点的无向稀疏图
    ReadGraph<SparseGraph> readGraph1(g1, filename);


    // 使用如下两种方式输出
    g1.show();
    for (int i = 0; i < g1.V(); ++i) {
        SparseGraph::adjIterator iter(g1, i);
        cout << "vertex " << i << ":\t";
        for (int w = iter.begin(); !iter.end(); w = iter.next())
            cout << w << " ";
        cout << endl;
    }

    DenseGraph g2(13, false); // 13 个节点的无向稠密图
    ReadGraph<DenseGraph> readGraph2(g2, filename);
    g2.show();

    // 计算联通分量
    // 稀疏图 g1 的联通分量
    Component<SparseGraph> c1(g1);
    cout << c1.count() << endl;
    
    // 稠密图 g2 的联通分量
    Component<DenseGraph> c2(g2);
    cout << c2.count() << endl;

    // DFS 求联通分量
    SparseGraph g3(7, false); // 7 个节点的无向稀疏图
    ReadGraph<SparseGraph> readGraph3(g3, "testG2.txt");
    g3.show();
    Component<SparseGraph> c3(g3);
    cout << c3.count() << endl;

    // DFS 求路径
    Path<SparseGraph> dfs(g3, 0);
    dfs.showPath(6);

    // BFS 求最短路径
    ShortestPath<SparseGraph> bfs(g3, 0);
    bfs.showPath(6);

}
