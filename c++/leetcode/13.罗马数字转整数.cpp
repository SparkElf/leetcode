/*
 * @lc app=leetcode.cn id=13 lang=cpp
 *
 * [13] 罗马数字转整数
 */
#include <string>
#include <unordered_map>
using namespace std;
// @lc code=start
class Solution {
public:
    unordered_map<char, int> dict = {
           {'I', 1},
           {'V', 5},
           {'X', 10},
           {'L', 50},
           {'C', 100},
           {'D', 500},
           {'M', 1000},
    };
    int romanToInt(string s) {
        int ans = 0;
        for (int i = 0;i < s.size();i++) {
            if (i < s.size() - 1 && dict[s[i]] < dict[s[i + 1]])ans -= dict[s[i]];
            else ans += dict[s[i]];
        }
        return ans;
    }
};
// @lc code=end

