/*
 * @lc app=leetcode.cn id=3 lang=javascript
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
/**
 * @param {string} s
 * @return {number}
 */
var lengthOfLongestSubstring = function (s) {
    let hash = new Array(256)//存储下标+1，以0为特殊字符
    let left = 0, right = 0, maxLen = 0

    while (right < s.length) {
        let ch = s[right]
        let index = hash[ch]
        if (index != 0 && index - 1 >= left) {//如果当前遍历到的字符已经在子串中 index>=left可以使得哈希表不用被重置
            if (right - left > maxLen)
                maxLen = right - left
            left = index //弹栈 注意下标已经+1
        }

        hash[ch] = right + 1
        right++
    }

    if (right - left > maxLen)
        maxLen = right - left  //漏了一次最后的最长子串计算
    return maxLen
};
// @lc code=end

