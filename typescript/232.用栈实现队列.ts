/*
 * @lc app=leetcode.cn id=232 lang=typescript
 *
 * [232] 用栈实现队列
 */

// @lc code=start
class MyQueue {
    stack = []
    queue = []
    constructor() {

    }

    push(x: number): void {
        this.stack.push(x)
        if (this.queue.length == 0)
            while (this.stack.length != 0)
                this.queue.push(this.stack.pop())
    }

    pop(): number {
        if (this.queue.length == 0)
            while (this.stack.length != 0)
                this.queue.push(this.stack.pop())
        return this.queue.pop()
    }

    peek(): number {
        if (this.queue.length == 0)
            while (this.stack.length != 0)
                this.queue.push(this.stack.pop())
        return this.queue[this.queue.length - 1]
    }

    empty(): boolean {
        return this.queue.length == 0 && this.stack.length == 0
    }
}

/**
 * Your MyQueue object will be instantiated and called as such:
 * var obj = new MyQueue()
 * obj.push(x)
 * var param_2 = obj.pop()
 * var param_3 = obj.peek()
 * var param_4 = obj.empty()
 */
// @lc code=end

