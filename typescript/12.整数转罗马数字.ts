/*
 * @lc app=leetcode.cn id=12 lang=typescript
 *
 * [12] 整数转罗马数字
 */

// @lc code=start
function intToRoman(num: number): string {
    //注意1<=num<=3999 那么最大的罗马数字为MMMCMXCIX
    let values = [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1]
    let chars = ["M", 'CM', "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"]
    let result = ""
    for (let i = 0; i < 13; i++)
        while (num >= values[i])
            num -= values[i], result += chars[i]
    return result
}
// @lc code=end

