#
# @lc app=leetcode.cn id=5 lang=python3
#
# [5] 最长回文子串
#

# @lc code=start

class Solution:
    def longestPalindrome(self, s: str) -> str:
        s = "#" + "#".join(s)+"#"  # 必须两头有#否则无法处理单字符
        sLen = len(s)
        lenArr = [0]*sLen
        c, r, p = 0, 0, 0
        for i in range(sLen):
            mirror = 2*c-i  # 对称回文中心的位置
            if i < r:
                if lenArr[mirror] < r-i:
                    continue
                else:
                    lenArr[i] = r-i
            b = i+(1+lenArr[i])  # 1的作用是指向边界之外
            a = i-(1+lenArr[i])
            if a >= 0 and b < sLen and s[a] == s[b]:
                lenArr[i] += 1
                a -= 1
                b += 1
                while a >= 0 and b < sLen and s[a] == s[b]:
                    lenArr[i] += 1
                    a -= 1
                    b += 1
                c = i
                r = i+lenArr[i]
                if lenArr[p] < lenArr[i]:
                    p = i
        result = s[p-lenArr[p]+1: p+lenArr[p]]
        result = result.replace("#", "")
        return result
        # @lc code=end
