/*
 * @lc app=leetcode.cn id=11 lang=typescript
 *
 * [11] 盛最多水的容器
 */

// @lc code=start
function maxArea(height: number[]): number {
    let maxC = 0
    let l = 0, r = height.length - 1
    let lastShort = 0
    while (l < r) {
        let short
        if (height[l] < height[r])
            short = height[l++]
        else
            short = height[r--]
        if (short > lastShort) {//短边变长了才计算容积
            lastShort = short
            let c = short * (r - l + 1)
            if (c > maxC)
                maxC = c
        }
    }
    return maxC
};
// @lc code=end

