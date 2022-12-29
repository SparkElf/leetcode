/*
 * @lc app=leetcode.cn id=11 lang=cpp
 *
 * [11] 盛最多水的容器
 */
#include <vector>
using namespace std;
// @lc code=start
class Solution {
public:
    int maxArea(vector<int>& height) {
        int i = 0, j = height.size() - 1, ans = 0;
        while (i < j)
            ans = height[i] < height[j] ? max(ans, (j - i) * height[i++]) : max(ans, (j - i) * height[j--]);
        return ans;
    }
};
// @lc code=end

