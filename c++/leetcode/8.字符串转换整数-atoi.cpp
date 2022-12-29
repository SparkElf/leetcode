/*
 * @lc app=leetcode.cn id=8 lang=cpp
 *
 * [8] 字符串转换整数 (atoi)
 */
#include <string>
#include <unordered_map>
using namespace std;
// @lc code=start

class Solution {
public:
    class Automation {
        enum STATE {
            START, SIGNED, NUMBER, END
        };
        unordered_map<STATE, vector<STATE>>table = {
            {STATE::START,{STATE::START,STATE::SIGNED,STATE::NUMBER,STATE::END}},
            {STATE::SIGNED,{STATE::END,STATE::END,STATE::NUMBER,STATE::END}},
            {STATE::NUMBER,{STATE::END,STATE::END,STATE::NUMBER,STATE::END}},
            {STATE::END,{STATE::END,STATE::END,STATE::END,STATE::END}}
        };
        STATE state = STATE::START;
        int col(char c) {
            if (c == ' ')return 0;
            else if (c == '+' || c == '-')return 1;
            else if (isdigit(c))return 2;
            return 3;
        }
    public:
        int sign = 1;
        long long ans = 0;
        void input(char c) {
            state = table[state][col(c)];
            if (state == STATE::NUMBER) {
                ans = ans * 10 + c - '0';
                ans = sign == 1 ? min(ans, (long long)INT_MAX) : min(ans, -(long long)INT_MIN);
            }
            else if (state == STATE::SIGNED)
                sign = c == '+' ? 1 : -1;
        }
    };
    int myAtoi(string s) {
        Automation automation;
        for (auto c : s)automation.input(c);
        return automation.sign * automation.ans;
    }
};
// @lc code=end

