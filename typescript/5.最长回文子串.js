/*
 * @lc app=leetcode.cn id=5 lang=javascript
 *
 * [5] 最长回文子串
 */

// @lc code=start
/**
 * @param {string} s
 * @return {string}
 */
var preprocess = function (s) {
    let str = new Array(s.length * 2 + 1)
    for (let i = 0; i < s.length; i++)
        str[2 * i] = '#', str[2 * i + 1] = s[i]
    str[str.length - 1] = '#'
    return str.join('')
}
var longestPalindrome = function (s) {
    let str = preprocess(s)
    //储存回文子串长度一半的数组
    let arr = new Array(str.length).fill(0)
    //当前回文中心
    let c = 0
    //回文子串右边界
    let r = 0
    //最长回文子串的中心位置
    let p = 0
    for (let i = 0; i < str.length; i++) {
        let mirror = 2 * c - i
        //以当前字符为回文中心的回文子串至少有这么长
        if (i < r) {
            //如果当前回文子串[严格]落在更大的回文子串内就肯定不会再扩展了
            if (arr[mirror] < r - i)
                continue
            else
                arr[i] = r - i
        }
        let a = i + (1 + arr[i])
        let b = i - (1 + arr[i])
        //扩展窗口 
        if (a < str.length && b >= 0 && str[a] == str[b]) {
            arr[i]++
            a++
            b--
            while (a < str.length && b >= 0 && str[a] == str[b]) {
                arr[i]++
                a++
                b--
            }
            //更新
            c = i;
            r = i + arr[i];
            if (arr[p] < arr[i])
                p = i
        }
    }

    //取得最终结果
    let result = str.substring(p - arr[p] + 1, p + arr[p])
    result = result.replace(/#/g, '')
    return result
};
// @lc code=end

