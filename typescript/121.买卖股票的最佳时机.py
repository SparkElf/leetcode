#
# @lc app=leetcode.cn id=121 lang=python3
#
# [121] 买卖股票的最佳时机
#

# @lc code=start
class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        minPrices = prices[0]
        maxProfit = 0
        for i in range(1, len(prices)):
            if prices[i]-minPrices > maxProfit:
                maxProfit = prices[i]-minPrices
            elif prices[i] < minPrices:
                minPrices = prices[i]
        return maxProfit
# @lc code=end
