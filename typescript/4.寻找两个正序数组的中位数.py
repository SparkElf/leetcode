#
# @lc app=leetcode.cn id=4 lang=python3
#
# [4] 寻找两个正序数组的中位数
#

# @lc code=start
from math import floor


class Solution:
    def findMedianSortedArrays(self, nums1: List[int], nums2: List[int]) -> float:
        m = len(nums1)
        n = len(nums2)
        if m > n:
            return self.findMedianSortedArrays(nums2, nums1)
        l, r = 0, m
        maxV, minV = 0, 0
        while l <= r:
            i = floor((l+r)/2)
            j = floor((m+n+1)/2)-i
            num_i_1 = float('-inf') if i == 0 else nums1[i-1]
            num_i = float('inf') if i == m else nums1[i]
            num_j_1 = float('-inf') if j == 0 else nums2[j-1]
            num_j = float('inf') if j == n else nums2[j]
            if num_i_1 <= num_j:
                maxV = max(num_i_1, num_j_1)
                minV = min(num_i, num_j)
                l = i+1
            else:
                r = i-1
        return (maxV+minV)/2 if (m+n) % 2 == 0 else maxV

# @lc code=end
