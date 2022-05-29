#
# @lc app=leetcode.cn id=796 lang=python3
#
# [796] 旋转字符串
#

# @lc code=start
class Solution:
    def rotateString(self, s: str, goal: str) -> bool:
        if len(s) != len(goal):  # 注意是len(s)而不是s.__len__，后者是方法而不是值
            return False
        temp = s+s
        if goal in temp:
            return True
        else:
            return False
# @lc code=end
