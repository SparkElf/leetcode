/*
 * @lc app=leetcode.cn id=8 lang=javascript
 *
 * [8] 字符串转换整数 (atoi)
 */

// @lc code=start
/**
 * @param {string} s
 * @return {number}
 */
var myAtoi = function (s) {
    let i = 0
    //丢弃前导空格
    while (s[i] == ' ') i++
    //判断是否是数字
    if (isNaN(s[i])) {
        if (s[i] != '-' && s[i] != '+') return 0
        else if (isNaN(s[i + 1])) return 0
    }

    let result = 0
    let MIN = -2147483648, MAX = 2147483647

    //判断正负号
    if (s[i] == '-') {
        result = - parseInt(s[++i]), i++
        while (i < s.length && !isNaN(s[i]) && s[i] != ' ')
            result = result * 10 - parseInt(s[i++])
    }
    else {
        if (s[i] == '+') i++
        while (i < s.length && !isNaN(s[i]) && s[i] != ' ')
            result = result * 10 + parseInt(s[i++])
    }

    if (result < MIN)
        result = MIN
    else if (result > MAX)
        result = MAX

    return result
}
// @lc code=end

