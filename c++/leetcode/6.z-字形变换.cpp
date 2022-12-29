/*
 * @lc app=leetcode.cn id=6 lang=cpp
 *
 * [6] Z 字形变换
 */
#include <string>
using namespace std;
// @lc code=start
class Solution {
public:
    string convert(string s, int numRows) {
        if (s.length() <= numRows || numRows <= 1)return s;
        int t = 2 * numRows - 2;
        string ans = "";
        for (int i = 0;i < numRows;i++)
            for (int j = 0;i + j < s.length();j += t) {
                ans += s[i + j];
                if (0 < i && i < numRows - 1 && t - i + j < s.length())
                    ans += s[t - i + j];
            }
        return ans;
    }
};
// @lc code=end

