/*
 * @lc app=leetcode.cn id=13 lang=typescript
 *
 * [13] 罗马数字转整数
 */

// @lc code=start
function romanToInt(s: string): number {
    let m = new Map()
    m.set("M", 1000)
    m.set("CM", 900)
    m.set("D", 500)
    m.set("CD", 400)
    m.set("C", 100)
    m.set("XC", 90)
    m.set("L", 50)
    m.set("XL", 40)
    m.set("X", 10)
    m.set("IX", 9)
    m.set("V", 5)
    m.set("IV", 4)
    m.set("I", 1)
    if (s.length == 1)
        return m.get(s[0])
    let res = 0
    for (let i = 0; i < s.length - 1; i++) {
        let key = s[i] + s[i + 1]
        if (m.get(key) != null)
            res += m.get(key), i++
        else
            res += m.get(s[i])
    }
    if (m.get(s[s.length - 2] + s[s.length - 1]) == null)
        res += m.get(s[s.length - 1])
    return res

};
// @lc code=end

