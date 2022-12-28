/*
 * @lc app=leetcode.cn id=4 lang=javascript
 *
 * [4] 寻找两个正序数组的中位数
 */

// @lc code=start
/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @return {number}
 */
var findMedianSortedArrays = function (nums1, nums2) {
    if (nums1.length > nums2.length)
        return findMedianSortedArrays(nums2, nums1)
    let m = nums1.length
    let n = nums2.length

    let l = 0, r = m
    let maxV = 0, minV = 0
    while (l <= r) {
        let i = Math.floor((l + r) / 2)//i+j刚好是两个数组元素总数的一半，这就是中位数的意义。
        let j = Math.floor((m + n + 1) / 2) - i
        let num_i_1 = (i == 0 ? Number.NEGATIVE_INFINITY : nums1[i - 1])
        let num_i = (i == m ? Number.POSITIVE_INFINITY : nums1[i])
        let num_j_1 = (j == 0 ? Number.NEGATIVE_INFINITY : nums2[j - 1])
        let num_j = (j == n ? Number.POSITIVE_INFINITY : nums2[j])
        if (num_i_1 <= num_j) {
            maxV = Math.max(num_i_1, num_j_1)
            minV = Math.min(num_i, num_j)
            l = i + 1
        }
        else
            r = i - 1
    }

    return (m + n) % 2 == 0 ? (maxV + minV) / 2.0 : maxV
};
// @lc code=end

