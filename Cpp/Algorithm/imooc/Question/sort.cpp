#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib> // rand()
#include "function.h"

using namespace std;

namespace mysort {
    template <typename T>
    string bubble_sort_1(vector<T> &arr) {
        int n = arr.size();
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n - i - 1; ++j)
                if (arr[j] > arr[j + 1])
                    std::swap(arr[j], arr[j + 1]);
        return string(__FUNCTION__);
    }

    template <typename T>
    string selection_sort(vector<T> &arr) { 
        int n = arr.size();
        for (int i = 0; i < n; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < n; ++j)
                if (arr[j] < arr[min_idx])
                    min_idx = j;
            std::swap(arr[i], arr[min_idx]);
        }
        return string(__FUNCTION__);
    }

    template <typename T>
    string insertion_sort(vector<T> &arr) {
        int n = arr.size();
        for (int i = 1; i < n; ++i) {
            int tmp = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > tmp) {
                arr[j + 1] = arr[j];
                j --;
            }
            arr[j + 1] = tmp;
        }
        return string(__FUNCTION__);
    }

    template <typename T>
    string shell_sort(vector<T> &arr) {
        int n = arr.size();
        int gap = n / 2;
        while (gap >= 1) {
            for (int i = gap; i < n; ++i) {
                int tmp = arr[i];
                int j = i - gap;
                while (j >= 0 && arr[j] > tmp) {
                    arr[j + gap] = arr[j];
                    j -= gap;
                }
                arr[j + gap] = tmp;
            }
            gap /= 2; // 注意不要忘了更新 gap
        }
        return string(__FUNCTION__);
    }


    template <typename T>
    void quick_sort_1(vector<T> &arr, int l, int r) {
        if (l >= r) return;

        std::swap(arr[l], arr[rand() % (r - l + 1) + l]);
        T v = arr[l];
        // arr[l+1, j] < v
        int j = l, i = l + 1;
        while (i <= r) {
            if (arr[i] < v)
                std::swap(arr[++j], arr[i]);
            ++i;
        }
        std::swap(arr[l], arr[j]);
        quick_sort_1(arr, l, j - 1);
        quick_sort_1(arr, j + 1, r);
    }

    template <typename T>
    string quick_sort_1ways(vector<T> &arr) {
        int n = arr.size();
        quick_sort_1(arr, 0, n - 1);
        return string(__FUNCTION__);
    }


    template <typename T>
    void quick_sort_2(vector<T> &arr, int l, int r) {

        if (l >= r) return;

        std::swap(arr[l], arr[rand() % (r - l + 1) + l]);

        T v = arr[l];
        // arr[l+1, i) <= v, arr(j, r] >= v
        int i = l + 1, j = r;
        while (true) {
            // 注意 while 中没有写 <= v 或 >= v 而是去掉了等于号
            // 是为了让两棵子树中等于 v 的元素尽量相等, 保存子树的平衡
            while (i <= r && arr[i] < v) ++i;
            while (j >= l + 1 && arr[j] > v) --j;
            if (i > j) break;
            std::swap(arr[i++], arr[j--]);
        }
        std::swap(arr[l], arr[j]);

        auto idx = j;
        quick_sort_2(arr, l, idx - 1);
        quick_sort_2(arr, idx + 1, r);
    }

    template <typename T>
    string quick_sort_2ways(vector<T> &arr) {
        int n = arr.size();
        quick_sort_2(arr, 0, n - 1);
        return string(__FUNCTION__);
    }


    template <typename T>
    void quick_sort_3(vector<T> &arr, int l, int r) {
        if (l >= r) return;

        std::swap(arr[l], arr[rand() % (r - l + 1) + l]);
        T v = arr[l];

        // arr[l+1...lt] < v, arr[lt+1...i) == v, arr[gt...r] > v
        int lt = l, i = l + 1, gt = r + 1;
        while (i < gt) {
            if (arr[i] == v) ++i;
            else if (arr[i] < v) std::swap(arr[++lt], arr[i++]);
            else std::swap(arr[--gt], arr[i]);
        }
        std::swap(arr[l], arr[lt]);

        quick_sort_3(arr, l, lt - 1);
        quick_sort_3(arr, gt, r);
    }

    template <typename T>
    string quick_sort_3ways(vector<T> &arr) {
        int n = arr.size();
        quick_sort_3(arr, 0, n - 1);
        return string(__FUNCTION__);
    }

    template <typename T>
    void merge(vector<T> &arr, int l, int r) {
        if (l >= r) return;
        int mid = l + (r - l) / 2;
        merge(arr, l, mid);
        merge(arr, mid+1, r);

        vector<T> res(r - l + 1);
        int k = 0, i = l, j = mid + 1;
        while (i <= mid && j <= r) {
            if (arr[i] < arr[j]) res[k++] = arr[i++];
            else res[k++] = arr[j++];
        }
        while (i <= mid) res[k++] = arr[i++];
        while (j <= r) res[k++] = arr[j++];
        for (int k = 0; k < res.size(); ++k)
            arr[l + k] = res[k];
    }

    template <typename T>
    string merge_sort(vector<T> &arr) {
        int n = arr.size();
        //merge(arr, 0, n - 1);
        merge(arr, 0, n - 1);
        return string(__FUNCTION__);
    }

    // 只适用于数据范围比较小的数组, 比如数组中元素表示成绩, 年龄等
    template <typename T>
    string counting_sort(vector<T> &arr) {
        T imax = *std::max_element(arr.begin(), arr.end());
        T imin = *std::min_element(arr.begin(), arr.end());
        vector<T> res(imax - imin + 1, 0);
        for (auto &i : arr)
            res[i - imin] ++;
        int k = 0;
        for (int i = 0; i < res.size(); ++i) {
            if (res[i]) {
                for (int j = 0; j < res[i]; ++j)
                    arr[k++] = i + imin;
            }
        }
        return string(__FUNCTION__);
    }


    template <typename T>
    ListNode<T>* bucket_insert(ListNode<T> *head, int val) {
        if (!head) return new ListNode<T>(val);
        auto dummy = new ListNode<T>(0);
        dummy->next = head;
        auto ptr = dummy;
        while (ptr->next && ptr->next->val <= val) ptr = ptr->next;
        ListNode<T> *node = new ListNode<T>(val);
        auto tmp = ptr->next;
        ptr->next = node;
        node->next = tmp;

        auto ret = dummy->next;
        delete dummy;
        return ret;
    }

    template <typename T>
    ListNode<T>* bucket_merge(ListNode<T> *l1, ListNode<T> *l2) {
        auto dummy = new ListNode<T>(0);
        auto ptr = dummy;
        while (l1 && l2) {
            if (l1->val < l2->val) {
                ptr->next = l1;
                l1 = l1->next;
            }
            else {
                ptr->next = l2;
                l2 = l2->next;
            }
            ptr = ptr->next;
        }
        ptr->next = l1 ? l1 : l2;

        auto ret = dummy->next;
        delete dummy;
        return ret;
    }


    template <typename T>
    string bucket_sort(vector<T> &arr) {
        int n = arr.size();
        const int BUCKET_NUM = 10;
        vector<ListNode<T>*> buckets(BUCKET_NUM, nullptr);
        for (int i = 0; i < n; ++i) {
            int idx = arr[i] % BUCKET_NUM;
            auto head = buckets[idx];
            buckets[idx] = bucket_insert(head, arr[i]);
        }

        ListNode<T> *dummy = new ListNode<T>(0);
        auto ptr = dummy;
        for (int i = 0; i < BUCKET_NUM; ++i) {
            //print_linked_list(buckets[i]);
            ptr->next = bucket_merge(ptr->next, buckets[i]);
            ptr = ptr->next;
        }

        ptr = dummy->next;
        int k = 0;
        while (ptr) {
            arr[k++] = ptr->val;
            ptr = ptr->next;
        }

        return string(__FUNCTION__);
    }

}



int main() {

    int m = 20;
    auto arr = generate_array<int>(m);
    auto brr = copy_array(arr);
    //print_array(arr);

    mysort::bucket_sort(arr);
    //print_array(arr);

    int n = 100000;
    //test_sort<int>(mysort::quick_sort_1ways<int>, n);
    //test_sort<int>(mysort::quick_sort_2ways<int>, n);
    //test_sort<int>(mysort::quick_sort_3ways<int>, n);
    //test_sort<int>(mysort::merge_sort<int>, n);
    //test_sort<int>(mysort::shell_sort<int>, n);
    //test_sort<int>(mysort::counting_sort<int>, n);
    test_sort<int>(mysort::bucket_sort<int>, n);

    //n = 10000;
    //test_sort<int>(mysort::insertion_sort<int>, n);
    //test_sort<int>(mysort::selection_sort<int>, n);
    //test_sort<int>(mysort::bubble_sort_1<int>, n);


}
