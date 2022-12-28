/*
 * @lc app=leetcode.cn id=6 lang=javascript
 *
 * [6] Z 字形变换
 */

// @lc code=start
/**
 * @param {string} s
 * @param {number} numRows
 * @return {string}
 */
var convert = function (s, numRows) {
    if (numRows === 1) return s
    let arr = new Array(s.length)
    let gap = 2 * numRows - 2//2*numRows- 1 -1
    let p = 0
    //|/|/|可以拆成若干/|的组合（除了第一列）
    //第一行的 就是/|的顶部
    for (let i = 0; i < s.length; i += gap)
        arr[p++] = s[i]
    //从第二行加到倒数第二行，如果加到倒数第一行会有重复
    //中间的 就是/|左右两边的点 
    for (let j = 1; j < numRows - 1; j++) {
        arr[p++] = s[j]//每行第一个
        for (let i = gap; i - gap < s.length; i += gap) {
            arr[p++] = s[i - j]//i就是尖尖的那个点
            if (i + j < s.length)
                arr[p++] = s[i + j]
        }
    }

    //最后一行
    for (let i = 0; numRows - 1 + i < s.length; i += gap)
        arr[p++] = s[numRows - 1 + i]
    return arr.join('')
};
// @lc code=end

