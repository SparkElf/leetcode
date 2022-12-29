/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */
#include <string>
#include <vector>
#include <iostream>
using namespace std;
// @lc code=start
class Solution {
public:
    string longestPalindrome(string s) {
        int len = s.size();
        if (len < 2)return s;

        auto dp = vector<vector<int>>(len, vector<int>(len));
        for (int i = 0;i < len;i++)dp[i][i] = 1;

        int maxLen = 1, begin = 0;
        for (int curLen = 2;curLen <= len;curLen++)
            for (int i = 0;i + curLen - 1 < len;i++) {
                int j = curLen + i - 1;
                if (s[i] != s[j])dp[i][j] = false;
                else {
                    if (j - i < 3)dp[i][j] = true;
                    else dp[i][j] = dp[i + 1][j - 1];
                }
                if (dp[i][j] && curLen > maxLen) {
                    maxLen = curLen;
                    begin = i;
                }
            }
        cout << maxLen << endl;
        return s.substr(begin, maxLen);
    }
};
// @lc code=end

