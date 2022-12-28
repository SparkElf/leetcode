#
# @lc app=leetcode.cn id=2 lang=python3
#
# [2] 两数相加
#

# @lc code=start
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
from math import floor


class Solution:
    def addTwoNumbers(self, l1: ListNode, l2: ListNode) -> ListNode:
        extra = 0
        cur = ListNode()
        root = cur
        while l1 is not None and l2 is not None:
            total = l1.val+l2.val+extra
            extra = floor(total/10)
            newNode = ListNode(total % 10)
            cur.next = newNode
            cur = newNode
            l1 = l1.next
            l2 = l2.next
        left = l2 if l1 is None else l1
        while left is not None:
            total = extra+left.val
            extra = floor(total/10)
            newNode = ListNode(total % 10)
            cur.next = newNode
            cur = newNode
            left = left.next
        if extra != 0:
            newNode = ListNode(extra)
            cur.next = newNode
        root = root.next
        return root
# @lc code=end
