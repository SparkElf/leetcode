/*
 * @lc app=leetcode.cn id=14 lang=cpp
 *
 * [14] 最长公共前缀
 */
#include <string>
#include <vector>
using namespace std;
// @lc code=start
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty())return "";

        for (int j = 0;j < strs[0].size();j++) {
            char c = strs[0][j];
            for (int i = 1;i < strs.size();i++)
                if (j >= strs[i].length() || strs[i][j] != c)
                    return strs[0].substr(0, j);
        }
        return strs[0];
    }
};
// @lc code=end

