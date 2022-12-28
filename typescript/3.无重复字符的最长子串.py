#
# @lc app=leetcode.cn id=3 lang=python3
#
# [3] 无重复字符的最长子串
#

# @lc code=start
class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:
        hash = [-1]*256
        left = 0
        right = 0
        maxLen = 0
        sLen = len(s)
        while(right < sLen):
            ch = s[right]
            index = hash[ord(ch)]
            if index != -1 and index >= left:
                print(ch, right, left)
                if right-left > maxLen:
                    maxLen = right-left
                left = index+1  # 弹栈到后一位
            hash[ord(ch)] = right
            right += 1
        if right-left > maxLen:
            maxLen = right-left
        return maxLen
        # @lc code=end
