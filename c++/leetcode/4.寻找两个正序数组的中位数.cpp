
/*
 * @lc app=leetcode.cn id=4 lang=cpp
 *
 * [4] 寻找两个正序数组的中位数
 */
#include<vector>
using namespace std;
// @lc code=start
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int totalLen = nums1.size() + nums2.size();
        if (totalLen % 2 == 1)return getKthElem(nums1, nums2, totalLen / 2 + 1);
        else return (getKthElem(nums1, nums2, totalLen / 2) + getKthElem(nums1, nums2, totalLen / 2 + 1)) / 2.0;
    }
    double getKthElem(const vector<int>& nums1, vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        int idx1 = 0, idx2 = 0;
        while (1) {
            if (idx1 == m)return nums2[idx2 + k - 1];
            if (idx2 == n)return nums1[idx1 + k - 1];
            if (k == 1)return min(nums1[idx1], nums2[idx2]);

            int newIdx1 = min(idx1 + k / 2 - 1, m - 1), newIdx2 = min(idx2 + k / 2 - 1, n - 1);
            if (nums1[newIdx1] <= nums2[newIdx2]) {
                k -= newIdx1 - idx1 + 1;//下标从0开始所以k要+1
                idx1 = newIdx1 + 1;
            }
            else {
                k -= newIdx2 - idx2 + 1;
                idx2 = newIdx2 + 1;
            }
        }
    }
};
// @lc code=end

