/*
 * @lc app=leetcode.cn id=1 lang=javascript
 *
 * [1] 两数之和
 */

// @lc code=start
/**
 * @param {number[]} nums
 * @param {number} target
 * @return {number[]}
 */
var twoSum = function (nums, target) {
    let m = new Map()
    for (let i = 0; i < nums.length; i++) {
        if (m.has(target - nums[i]))
            return [m.get(target - nums[i]), i]
        m.set(nums[i], i)
    }
    return []
};
// @lc code=end

