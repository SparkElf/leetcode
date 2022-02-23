#
# @lc app=leetcode.cn id=101 lang=python3
#
# [101] 对称二叉树
#

# @lc code=start
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
def isMirror(left: TreeNode, right: TreeNode):
    if left is None and right is None:
        return True
    elif left is None or right is None:
        return False
    else:
        return left.val == right.val and isMirror(left.left, right.right) and isMirror(left.right, right.left)


class Solution:
    def isSymmetric(self, root: TreeNode) -> bool:
        if root == None:
            return True
        else:
            return isMirror(root.left, root.right)

# @lc code=end
