#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <cassert>


using namespace std;


// Dense Graph : Adjacency Matrix
class DenseGraph {

private:
    int n, m; // n 为节点数, m 为边数
    bool directed; // 是否为有向图
    vector<vector<bool>> g; // 邻接矩阵
public:
    DenseGraph(int n, bool directed) {
        this->n = n;
        this->m = 0;
        this->directed = directed;
        for (int i = 0; i < n; ++i)
            g.push_back(vector<bool>(n, false));
    }
    ~DenseGraph() {}

    // 返回节点与边的数量
    int V() const { return n; }
    int E() const { return m; }

    // 传入两个顶点的索引
    // 使用邻接矩阵可以很方便的处理平行边的情况, 如果两个
    // 顶点已经有边了, 那么直接返回.
    void addEdge(int v, int w) {
        assert( v >= 0 && v < n );
        assert( w >= 0 && w < n );

        // 处理平行边的情况
        if (hasEdge(v, w))
            return;

        g[v][w] = true; // 设置 v 和 w 的边
        if (!directed) // 如果是无向图
            g[w][v] = true;
        // 注意维护边的数量, 另外在 m++ 之前需要使用 hasEdge
        // 判断两个顶点是否存在边, 否则 m 会被多次增加.
        m ++;
    }

    bool hasEdge(int v, int w) {
        assert( v >= 0 && v < n );
        assert( w >= 0 && w < n );
        return g[v][w];
    }

    void show() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < g[i].size(); ++j)
                cout << g[i][j] << " ";
            cout << endl;
        }
    }

    // 稠密图的迭代器
    class adjIterator {
    private:
        DenseGraph &G;
        int v; // 当前访问的节点
        int index; // 此时已经迭代到何处了
    public:
        // 传递的参数为要访问的图, 以及要访问节点 v 的邻边.
        adjIterator(DenseGraph &graph, int v) : G(graph) {
            this->v = v;
            this->index = -1;
        }

        // 注意 index 一开始为 -1, 因为要找到 hasEdge(v, w) 为 true
        // 的第一个节点 w.
        int begin() {
            index = -1;
            return next();
        }

        int next() {

            for (index += 1; index < G.V(); index ++)
                if (G.g[v][index]) // 判断是否为 true
                    return index;
            return -1;
        }

        bool end() {
            return index >= G.V(); // 是否大于顶点的个数.
        }
    };

};


// Sparse Graph : Adjacency Lists
class SparseGraph {

private:
    int n, m;
    bool directed;
    vector<vector<int>> g; // 邻接表
public:
    SparseGraph(int n, bool directed) {
        this->n = n;
        this->m = 0;
        this->directed = directed;
        for (int i = 0; i < n; ++i)
            g.push_back(vector<int>{});
    }

    ~SparseGraph() {}

    // 返回节点与边的数量
    int V() const { return n; }
    int E() const { return m; }

    void addEdge(int v, int w) {
        assert( v >= 0 && v < n );
        assert( w >= 0 && w < n );

        // 使用邻接表来判断是否存在平行边时间复杂度比较高,
        // 因此 addEdge 里面暂时不考虑平行边的情况, 见下面的 hasEdge.

        g[v].push_back(w);
        // 若为无向图, 并且要处理自环边的情形
        if ( v != w && !directed)
            g[w].push_back(v);

        m ++;
    }

    // 使用邻接表判断两个顶点是否有边的时间复杂度为 O(n)
    bool hasEdge(int v, int w) {
        assert( v >= 0 && v < n );
        assert( w >= 0 && w < n );

        for (int i = 0; i < g[v].size(); ++i)
            if (g[v][i] == w)
                return true;
        return false;
    }
    
    void show() {
        for (int i = 0; i < n; ++i) {
            cout << "vertex " << i << ":\t";
            for (int j = 0; j < g[i].size(); ++j)
                cout << g[i][j] << " ";
            cout << endl;
        }
    }

    // 稀疏图的迭代器
    class adjIterator {
    private:
        SparseGraph &G;
        int v; // 当前访问的节点
        int index; // 此时已经迭代到何处了
    public:
        // 传递的参数为要访问的图, 以及要访问节点 v 的邻边.
        adjIterator(SparseGraph &graph, int v) : G(graph) {
            this->v = v;
            this->index = 0;
        }

        int begin() {
            index = 0; // begin() 可能会被多次调用
            if (G.g[v].size())
                return G.g[v][index];
            return -1;
        }

        int next() {
            index ++;
            if (index < G.g[v].size())
                return G.g[v][index];
            return -1;
        }

        bool end() {
            return index >= G.g[v].size();
        }
    };

};

#endif
