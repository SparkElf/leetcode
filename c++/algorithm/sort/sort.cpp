#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void insertion_sort(int* a, int len) {
    for (int i = 1; i < len; i++)
        for (int j = i;j - 1 >= 0;j--)
            if (a[j] < a[j - 1])swap(a[j], a[j - 1]);
            else break;
}
void bubble_sort(int* a, int len) {
    bool sorted = true;
    for (int j = len;j > 1 && !sorted;j--) {
        sorted = true;
        for (int i = 1;i < j;i++)
            if (a[i - 1] > a[i]) {
                swap(a[i], a[i - 1]);
                sorted = false;
            }
    }

}
void selection_sort(int* a, int len) {
    for (int i = 0;i < len - 1;i++) {
        int p = i;
        for (int j = i + 1;j < len;j++)
            if (a[j] < a[p])p = j;
        swap(a[p], a[i]);
    }
}
void shell_sort(int* a, int len) {
    for (int gap = len >> 1; gap > 0; gap >>= 1)
        for (int i = gap;i < len;i++) {
            //对间隔为gap的序列进行插入排序
            for (int j = i;j - gap >= 0;j -= gap)
                if (a[j] < a[j - gap])swap(a[j], a[j - gap]);
                else break;
        }
}
void quick_sort(int* a, int l, int r) {
    if (l >= r)return;
    swap(a[l], a[rand() % (r - l + 1) + l]);
    int i = l, j = r;//l+1不行
    int pivot = a[l];
    while (i < j) {
        while (i < j && a[j] >=pivot)j--;
        a[i] = a[j];
        while (i < j && a[i] <=pivot)i++;
        a[j] = a[i];
    }
    a[i] = pivot;
    quick_sort(a, l, j - 1);
    quick_sort(a, j + 1, r);
}
/*  //归并排序
    vector<int> sortArray(vector<int>& a) {
    int*temp=new int[a.size()];
    int* res = merge_sort(a.data(), temp, a.size());
    if (res != a.data())
        for (int i = 0;i < a.size();i++)
            a[i] = res[i];
    return a;
} */
int* merge_sort(int* a, int* temp, int len) {
    if (len == 1) return a;
    int mid = len >> 1;
    //切分
    int* p1 = merge_sort(a, temp, mid);
    int* p2 = merge_sort(a + mid, temp + mid, len - mid);
    if (p1 != a)swap(a, temp);
    //奇数比偶数多了一层
    if (p1 != p2 - mid)
        for (int i = 0;i < len - mid;i++)
            p1[i + mid] = p2[i];

    //合并
    int i = 0, j = mid, k = 0;
    while (i < mid && j < len) {
        if (a[i] < a[j])temp[k] = a[i++];
        else temp[k] = a[j++];
        k++;
    }
    while (i < mid)temp[k++] = a[i++];
    while (j < len)temp[k++] = a[j++];

    return temp;
}

namespace heap_sort {
    void siftDown(int i, int* a, int len) {
        while (i  < len) {
            int l = i * 2 + 1, r = i * 2 + 2;
            int large = i;
            if (l<len && a[l] > a[large])large = l;
            if (r<len && a[r]>a[large])large = r;
            if (large != i) {
                swap(a[i], a[large]);
                i = large;
            }
            else
                break;
        }
    }
    void heap_sort(int* a, int len) {
        //建堆
        for (int i = len / 2;i >= 0;i--)
            siftDown(i, a, len);
        //根节点和最后一个元素交换，做堆调整
        for (int i = len - 1;i >= 1;i--) {
            swap(a[i], a[0]);
            siftDown(0, a, i);
        }
    }
}
/*  //计数排序
    vector<int> sortArray(vector<int>& a) {
    counting_sort(a.data(),a.size());
    return a;
}
*/
void counting_sort(int* a, int len) {
    int maxV = a[0], minV = a[0];
    for (int i = 1;i < len;i++)
        if (a[i] > maxV)maxV = a[i];
        else if (a[i] < minV)minV = a[i];
    int range = maxV - minV + 1;
    int* b = new int[range]();

    for (int i = 0;i < len;i++)
        b[a[i] - minV] += 1;
    int k = 0;
    for (int i = 0;i < range;i++)
        while (b[i] -- > 0)a[k++] = i + minV;
    delete[] b;
}
void bucket_sort(int* a, int len) {
    if (len == 1)return;
    int maxV = a[0], minV = a[0], gap = 0;
    for (int i = 1;i < len;i++) {
        if (a[i] > maxV)maxV = a[i];
        else if (a[i] < minV)minV = a[i];
        gap += abs(a[i] - a[i - 1]);
    }
    gap = 2 * gap / (len - 1);//一个桶500个元素
    vector<vector<int>>b((maxV - minV) / gap + 1);
    for (int i = 0;i < len;i++)
        b[(a[i] - minV) / gap].push_back(a[i]);
    for (int i = 0;i < b.size();i++)
        sort(b[i].data(), b[i].data() + b[i].size());
    for (int k = 0, i = 0;i < b.size();i++)
        for (int j = 0;j < b[i].size();j++)
            a[k++] = b[i][j];
}
void radix_sort(int* a, int len) {
    //找最大位数
    int maxV = a[0];
    for (int i = 1;i < len;i++)if (a[i] > maxV)maxV = a[i];
    int d = 1;
    while (maxV /= 10)d++;
    vector<int> bucket[19];//正负数共十九位
    int pow = 1;
    while (d--) {
        for (int i = 0;i < 19;i++) bucket[i].clear();
        for (int i = 0;i < len;i++) bucket[(a[i] / pow) % 10 + 9].push_back(a[i]);
        int k = 0;
        for (int i = 0;i < 19;i++)
            for (int x : bucket[i])
                a[k++] = x;
        pow *= 10;
    }
}
//https://leetcode.cn/problems/kth-largest-element-in-an-array/
//复杂度分析 https://wenku.baidu.com/view/b56646bef8d6195f312b3169a45177232e60e463.html?_wkts_=1668145054259&bdQuery=%E9%9A%8F%E6%9C%BA%E5%BF%AB%E9%80%9F%E9%80%89%E6%8B%A9%E7%9A%84%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6 小于3n
int findKthLargest(vector<int>& nums, int k) {
    return partion(nums.data(), 0, nums.size() - 1, k);
}
int partion(int* a, int l, int r, int k) {
    if (l >= r)return a[l];
    swap(a[l], a[rand() % (r - l + 1) + l]);
    int i = l, j = r;
    int pivot = a[l];
    while (i < j) {
        while (i < j && a[j] <=pivot)j--;
        a[i] = a[j];
        while (i < j && a[i]>=pivot)i++;
        a[j] = a[i];
    }
    a[i] = pivot;
    if (i == k - 1)return a[j];
    else if (i > k - 1)return partion(a, l, i - 1, k);
    return partion(a, i + 1, r, k);
}

int main() {

    return 0;
}