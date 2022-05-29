/*
 * @lc app=leetcode.cn id=7 lang=javascript
 *
 * [7] 整数反转
 */

// @lc code=start
/**
 * @param {number} x
 * @return {number}
 */
var reverse = function (x) {
    let result = 0
    let MIN = -214748364//2 147 483 648
    let MAX = 214748364
    while (x != 0) {
        if (MIN <= result && result <= MAX)//缩小十倍比较
            result = result * 10 + x % 10
        else
            return 0
        x = ~~(x / 10)//向下取整
    }
    return result
};

// @lc code=end

