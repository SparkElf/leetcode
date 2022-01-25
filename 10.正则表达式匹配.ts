/*
 * @lc app=leetcode.cn id=10 lang=typescript
 *
 * [10] 正则表达式匹配
 */

// @lc code=start
class Node {
    value: string
    next: Node//不会发生回退
    pre: Node
    hasBack: boolean
    constructor(value: string) {
        this.value = value
    }
    delete() {//可能有内存泄漏但是不要紧，随正则表达式实例整体删除
        if (this.pre != null)
            this.pre.next = this.next
        if (this.next != null)
            this.next.pre = this.pre
    }
}
class RegEngine_NFA {
    head: Node
    //tail一定满足tail.next==null
    constructor(pattern: string) {
        let head = RegEngine_NFA.Reg2NFA(pattern)
        this.head = head
    }
    static Reg2NFA(s: string): Node {
        let head = new Node(s[0])
        let pre = head
        for (let i = 1; i < s.length; i++) {
            let node = new Node(s[i])
            pre.next = node
            node.pre = pre
            pre = node
        }
        let cur: Node
        for (cur = head; cur != null; cur = cur.next) {
            //上一个节点成环 A<->A 删除*节点
            if (cur.value == '*') {
                cur.pre.hasBack = true
                cur.delete()//cur.next==cur.pre.next因为没置空
            }
        }
        //清除冗余节点
        for (cur = head.next; cur != null; cur = cur.next)
            if (cur.pre.hasBack && cur.hasBack && (cur.pre.value == cur.value || cur.pre.value == '.'))
                cur.delete()

        cur = head
        while (cur.next != null) cur = cur.next

        while (cur.pre != null) {
            if (cur.pre.hasBack && cur.hasBack && cur.value == '.')
                cur.pre.delete()
            cur = cur.pre
            if (cur == null) break
        }

        return head
    }
    match(s: string): boolean {
        return this.matchImpl(s, 0, this.head)
    }
    private matchImpl(s: string, index: number, cur: Node): boolean {
        //字符串指针不动*的特性：不匹配也可以走 //先跳过星号方便编写
        if (cur.hasBack && cur.next != null && this.matchImpl(s, index, cur.next))
            return true
        if (s[index] == cur.value || cur.value == '.') {//字符串指针后移
            if (index == s.length - 1) {//要保证可以回溯
                for (let n = cur.next; n != null; n = n.next)//a ab*b*b* 把所有可选路径走完 要求终结于可选路径
                    if (!n.hasBack)
                        return false
                return true
            }
            else {
                if (cur.next != null && this.matchImpl(s, index + 1, cur.next))//字符串没遍历完
                    return true
                if (cur.hasBack && this.matchImpl(s, index + 1, cur))
                    return true
            }
        }
        return false
    }
}
function isMatch(s: string, p: string): boolean {
    let regex = new RegEngine_NFA(p)
    return regex.match(s)
};
// @lc code=end
isMatch("bbbba", ".*a*a")