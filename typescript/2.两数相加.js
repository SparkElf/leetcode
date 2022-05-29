/*
 * @lc app=leetcode.cn id=2 lang=javascript
 *
 * [2] 两数相加
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * function ListNode(val, next) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.next = (next===undefined ? null : next)
 * }
 */
/**
 * @param {ListNode} l1
 * @param {ListNode} l2
 * @return {ListNode}
 */
var addTwoNumbers = function (l1, l2) {
    let extra = 0//
    let cur = new ListNode()
    let root = cur
    while (l1 != null && l2 != null) {
        let sum = l1.val + l2.val + extra
        extra = Math.floor(sum / 10)
        let newNode = new ListNode(sum % 10)
        cur.next = newNode
        cur = newNode
        l1 = l1.next
        l2 = l2.next
    }

    let left
    if (l1 == null) left = l2
    else left = l1

    while (left != null) {
        let sum = extra + left.val
        extra = Math.floor(sum / 10)
        let newNode = new ListNode(sum % 10)
        cur.next = newNode
        cur = newNode
        left = left.next
    }
    if (extra != 0) {
        let newNode = new ListNode(extra)
        cur.next = newNode
    }
    root = root.next

    return root
};
// @lc code=end

