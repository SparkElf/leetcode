/*
 * @lc app=leetcode.cn id=9 lang=javascript
 *
 * [9] 回文数
 */

// @lc code=start
/**
 * @param {number} x
 * @return {boolean}
 */
var isPalindrome = function (x) {
    //找一些边界条件加速算法
    //x小于0时必定不是回文数
    //10 1000 10000这种会影响后续算法的处理 因为它翻转后第一位是0
    if (x < 0 || (x % 10 == 0 && x != 0))
        return false
    let revertedNum = 0
    while (x > revertedNum) {
        revertedNum = revertedNum * 10 + x % 10
        x = ~~(x / 10)
    }
    return x == revertedNum || x == ~~(revertedNum / 10)
};
// @lc code=end

