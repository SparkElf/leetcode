#
# @lc app=leetcode.cn id=56 lang=python3
#
# [56] 合并区间
#

# @lc code=start


class Solution:
    def merge(self, intervals: List[List[int]]) -> List[List[int]]:
        # List().sort()
        intervals.sort(key=lambda x: x[0])  # 按照左端点排序
        merged = []
        merged.append(intervals[0])
        for i in range(1, len(intervals)):
            if merged[-1][1] < intervals[i][0]:  # 如果merged中最后一个数的右端点小于当前遍历到的左端点则区间不重合
                merged.append(intervals[i])
            else:
                # 否则区间重合，更新右端点，左端点不用更新
                merged[-1][1] = max(merged[-1][1], intervals[i][1])
        return merged
        # @lc code=end
