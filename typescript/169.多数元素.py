#
# @lc app=leetcode.cn id=169 lang=python3
#
# [169] 多数元素
#

# @lc code=start
class Solution:
    def majorityElement(self, nums: List[int]) -> int:
        represent = nums[0]
        vote = 1
        for i in range(1, len(nums)):
            if nums[i] == represent:
                vote += 1
            else:
                vote -= 1
                if vote < 0:
                    represent = nums[i]
                    vote = 1
        return represent
# @lc code=end
