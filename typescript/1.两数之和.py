#
# @lc app=leetcode.cn id=1 lang=python3
#
# [1] 两数之和
#

# @lc code=start
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        m = {}
        for i in range(len(nums)):
            if m.get(target-nums[i]) is not None:
                return [m.get(target-nums[i]), i]
            m[nums[i]] = i
        return []
# @lc code=end
