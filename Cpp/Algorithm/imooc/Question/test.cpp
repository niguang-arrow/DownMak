// 实现一棵二叉树, 注意, 不是二叉搜索树, 所以插入的时候使用的
// 是广度优先的遍历方法.
#include <iostream>
#include <queue>
#include <initializer_list>
#include <sstream>
#include <string>
#include <iomanip>
#include <cassert>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    TreeNode *root;
    
    void preOrder(TreeNode *node) {
        if (!node)
            return;
        cout << node->val << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() {
        // TODO: 这个析构函数比较麻烦, 暂时不管
    }
    BinaryTree(initializer_list<int> element) : root(nullptr) {
        for (int d : element)
            insert(d);
    }

    void insert(int num) {
        if (!root) {
            root = new TreeNode(num);
            return;
        }

        queue<TreeNode*> Queue;
        Queue.push(root);
        while (!Queue.empty()) {
            TreeNode *node = Queue.front();
            Queue.pop();
            if (!node->left) {
                node->left = new TreeNode(num);
                return;
            }
            if (!node->right) {
                node->right = new TreeNode(num);
                return;
            }
            Queue.push(node->left);
            Queue.push(node->right);
        }
    }

    void preOrder() {
        if (root) {
            preOrder(root);
            cout << endl;
        }
    }
};

// 并查集的第一种实现, union (注意union是关键字)操作的时间复杂度
// 为 O(n). 而 find 操作为 O(1), 因此这种实现方法为 Quick Find.
namespace UnionFind1 {
    class UnionFind {
        private:
            int *id;
            int count;

        public:
            UnionFind(int n) : count(n) {
                id = new int[n];
                for (int i = 0; i < n; ++i)
                    id[i] = i;
            }

            ~UnionFind() { delete[] id; }

            // 查找元素为 p 的 id 号
            int find(int p) {
                assert(p >= 0 && p < count);
                return id[p];
            }

            // 元素 p 和 q 是否连接在一起
            bool isConnected(int p, int q) {
                return find(p) == find(q);
            }
            
            // 将两个元素并在一起
            void unionElement(int p, int q) {
                int pId = find(p);
                int qId = find(q);

                if (pId == qId)
                    return;
                
                // 使得和 p 同一个组的元素并到 q 元素所在的组中
                for (int i = 0; i < count; ++i)
                    if (id[i] == pId)
                        id[i] = qId;
            }
    };
}

// 每一个元素都有一个指向父节点的指针, 而根节点的指针指向自身.
// Quick Union


void merge(vector<int> &nums, int left, int mid, int right) {
    if (nums.empty() || left >= right)
        return;
    vector<int> array(right - left + 1);
    int k = 0; // 用于遍历 array
    int l = left, r = mid + 1;
    while (l <= mid && r <= right) {
        if (nums[l] < nums[r])
            array[k++] = nums[l++];
        else
            array[k++] = nums[r++];
    }
    while (l <= mid)
        array[k++] = nums[l++];
    while (r <= right)
        array[k++] = nums[r++];

    for (int i = 0; i < array.size(); ++i)
        nums[left + i] = array[i];
    return;
}

void mergeSort(vector<int> &nums, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    cout << mid << endl;
    mergeSort(nums, left, mid);
    mergeSort(nums, mid + 1, right);
    merge(nums, left, mid, right);
}

void mergeSort(vector<int> &nums) {
    if (nums.empty())
        return;
    mergeSort(nums, 0, nums.size() - 1);
}

int main() {
    
    //BinaryTree tree({1, 2, 3, 4, 5});
    //tree.preOrder();
    ////cout << tree.root->val << endl;
    ////cout << tree.root->left->val << endl;
    ////cout << tree.root->right->val << endl;
    //cout << std::max(10, 11) << endl;

    //stringstream ss("abc abc bbb bbb"), end;
    //string s;
    //while (ss >> s) {
        //cout << s << " ";
    //}
    //cout << std::boolalpha << ss.eof() << endl;
    //cout << endl;

    int arr[] = {1, 3, 5, 2, 6, 3, 4, 7};
    vector<int> nums(arr, arr + sizeof(arr)/sizeof(int));
    mergeSort(nums);
    for (auto &d : nums)
        cout << d << " ";
    cout << endl;

}
