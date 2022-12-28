#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace ListAlgorithm {
	/*
	* 合并两个递增的有序链表，要求空间复杂度为O(1)  https://leetcode.cn/problems/merge-two-sorted-lists/
	*/
	struct ListNode {
		int val;
		ListNode* next;
		ListNode(int x) : val(x), next(NULL) {}
	};
	ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
		ListNode ans = ListNode(0);
		ListNode* cur = &ans, ** op = NULL;
		while (l1 && l2) {
			if (l1->val < l2->val) op = &l1;
			else op = &l2;
			cur->next = *op;
			*op = (*op)->next;
			cur = cur->next;
		}
		cur->next = l1 ? l1 : l2;//不用再while拼接了
		return ans.next;
	}
	/*
		合并k个有序链表 https://leetcode.cn/problems/vvXgSW/
	*/
	ListNode* mergeKLists1(vector<ListNode*>& lists) {
		ListNode* ans = nullptr;
		for (int i = 0; i < lists.size(); i++)
			ans = mergeTwoLists(ans, lists[i]);
		return ans;
	}
	struct Status {
		int val;
		ListNode* ptr;
		const bool operator< (const Status& r)const {
			return val > r.val;
		}
	};
	ListNode* mergeKList2(vector<ListNode*>& lists) {
		priority_queue<Status>q;
		for (auto node : lists)if (node)q.push({ node->val,node });
		ListNode ans(0);
		ListNode* cur = &ans;
		while (!q.empty()) {
			auto s = q.top(); q.pop();
			cur->next = s.ptr;
			cur = cur->next;
			if (s.ptr->next)q.push({ s.ptr->next->val,s.ptr->next });
		}
		return ans.next;
	}
	/*
	* 删除重复元素 递增
	*/
	ListNode* mergeTwoListsUnique(ListNode* l1, ListNode* l2) {
		ListNode ans = ListNode(0);
		ListNode* cur = &ans;
		while (l1 && l2) {
			if (l1->val < l2->val) {
				cur->next = l1;
				l1 = l1->next;
			}
			else if (l1->val > l2->val) {
				cur->next = l2;
				l2 = l2->next;
			}
			else {
				cur->next = l1;
				l1 = l1->next;
				ListNode* temp = l2->next;
				delete l2;
				l2 = temp;
			}
			cur = cur->next;
		}
		cur->next = l1 ? l1 : l2;//不用再while拼接了
		return ans.next;
	}
	/*
	*   将两个非递减的链表合并为一个非递增的有序链表，空间复杂度O(1)，允许元素重复。
	*/
	ListNode* mergeTwoListsReverse(ListNode* l1, ListNode* l2) {
		//基本思路 采用头插法
		ListNode* ans = nullptr, * temp = nullptr, ** op = nullptr;
		while (l1 && l2) {
			if (l1->val < l2->val) op = &l1;
			else op = &l2;
			temp = (*op)->next;
			(*op)->next = ans;
			ans = *op;
			*op = temp;
		}
		if (l1)op = &l1;
		else op = &l2;
		while (*op) {
			temp = (*op)->next;
			(*op)->next = ans;
			ans = *op;
			*op = temp;
		}
		return ans;
	}
	/*
	*   删除指定链表下标a到b元素后把list2拼接到缺口，链表下标从1开始 难点在于头尾结点边界情况 https://leetcode.cn/problems/merge-in-between-linked-lists/
	*/
	ListNode* mergeInBetween(ListNode* list1, int a, int b, ListNode* list2) {
		int i = -1;
		ListNode head(0);
		head.next = list1;
		ListNode* l = &head, * r = nullptr;
		while (l && i + 1 != a) {
			l = l->next;
			i++;
		}
		r = l->next;
		i++;
		while (r && i != b + 1) {
			auto temp = r->next;
			delete r;
			r = temp;
			i++;
		}
		l->next = list2;
		while (list2->next)list2 = list2->next;
		list2->next = r;
		return head.next;
	}
	/*
	*   反转链表 头插法 https://leetcode.cn/problems/UHnkqh/
	*/
	ListNode* reverseList(ListNode* head) {
		ListNode* ans = nullptr;
		while (head) {
			auto temp = head->next;
			head->next = ans;
			ans = head;
			head = temp;
		}
		return ans;
	}
	/*
	*   找到链表的中点 https://leetcode.cn/problems/middle-of-the-linked-list/solution/lian-biao-de-zhong-jian-jie-dian-by-leetcode-solut/
	*/
	ListNode* middleNode(ListNode* head) {
		//快慢指针法还可延伸到k等分点
		//slow奇数时停在中点，偶数停在后半段开头
		//要想停在中点前段，修改判断条件为 fast->next!=nullptr && fast->next->next!=nullptr
		ListNode* slow = head;
		ListNode* fast = head;
		while (fast != nullptr && fast->next != nullptr) {
			slow = slow->next;
			fast = fast->next->next;
		}
		return slow;
	}
	ListNode* middleNodePre(ListNode* head) {
		if (head == nullptr)return nullptr;
		//快慢指针法还可延伸到k等分点
		//slow奇数时停在中点，偶数停在后半段开头
		//要想停在中点前段，修改判断条件为 fast->next!=nullptr && fast->next->next!=nullptr
		ListNode* slow = head;
		ListNode* fast = head;
		while (fast->next != nullptr && fast->next->next != nullptr) {
			slow = slow->next;
			fast = fast->next->next;
		}
		return slow;
	}
	/*
	* 综合题 把链表重排成1 n 2 n-1 ... https://leetcode.cn/problems/LGjMqU/
	*/
	void reorderList(ListNode* head) {
		if (head == nullptr)return;
		ListNode* l1, * l2, * mid;
		//找到中间结点
		mid = middleNode(head);
		l2 = mid->next;
		mid->next = nullptr;
		l1 = head;
		//反转后半部分
		l2 = reverseList(l2);
		//拼接
		while (l2) {
			auto temp1 = l1->next;
			auto temp2 = l2->next;
			l1->next = l2;
			l2->next = temp1;
			l1 = temp1;
			l2 = temp2;
		}
	}
	/*
	*   反转局部链表 下标从1开始 https://leetcode.cn/problems/reverse-linked-list-ii/
	*/
	ListNode* reverseBetween(ListNode* head, int left, int right) {
		int i = 0;
		ListNode ans(0);
		ans.next = head;
		ListNode* l = &ans;
		while (l && i + 1 != left) {
			l = l->next;
			i++;
		}
		ListNode* r = l;
		while (r && i != right) {
			r = r->next;
			i++;
		}
		ListNode* h = r, * t = l->next;
		r = r->next;
		h->next = nullptr;
		reverseList(l->next);
		auto temp = h;
		while (temp) {
			cout << temp->val << endl;
			temp = temp->next;
		}
		l->next = h;
		t->next = r;
		return ans.next;
	}
	/*
	*   A、B链表求交集，交集存储在A链表中，元素保持递增 旧链表元素不要要删除
	*/
	ListNode* Intersection(ListNode* l1, ListNode* l2) {
		ListNode ans(0);
		ListNode* cur = &ans, ** op;
		while (l1 && l2) {
			if (l1->val == l2->val) {
				cur->next = l1;
				l1 = l1->next;
				l2 = l2->next;//两两配对
				cur = cur->next;
			}
			if (l1->val < l2->val) op = &l1;
			else op = &l2;
			*op = (*op)->next;
		}
		return ans.next;
	}
	/*
	*   链表求差 A-B ，求差后结果存储在A中，并求求差后的元素个数
	*/
	int Difference(ListNode* l1, ListNode* l2) {
		//注意求差不是一一配对的 2222 2 求差的结果是空链表
		ListNode h1(0), h2(0);
		h1.next = l1, h2.next = l2;
		ListNode* temp = nullptr, * pre = &h1;;
		int n = 0;
		while (l1 && l2) {
			if (l1->val < l2->val) {
				n++;
				pre = l1;
				l1 = l1->next;
			}
			else if (l1->val > l2->val)l2 = l2->next;
			else {
				pre->next = l1->next;
				auto temp = l1;
				l1 = l1->next;
				delete temp;
			}
		}
	}
	/*
	* 设计一个算法把A分解为B、C，其中B中为A中小于0的结点，C中为A中大于0的结点
	*/
	void Decompose(ListNode* A, ListNode*& B, ListNode*& C) {
		//新建头结点
		B = new ListNode(0), C = new ListNode(0);
		ListNode* pb = B, * pc = C;
		while (A) {
			if (A->val < 0) {
				pb->next = A;
				pb = A;
			}
			else if (A->val > 0) {
				pc->next = A;
				pc = A;
			}
			A = A->next;
		}
		pb->next = nullptr;
		pc->next = nullptr;
		auto temp = B;
		B = B->next;
		delete temp;
		temp = C;
		C = C->next;
		delete temp;
	}
	/**
	 *  设计一个算法O(n)时间删除顺序表中所有值为item的元素，空间复杂度O(1)
	*/
	void delItem(vector<int> a, int item) {
		int k = 0;
		for (int i = 0; i < a.size(); i++)
			if (a[i] != item)
				a[k++] = a[i];
		a.resize(k);
	}
	/**
	 *  找到倒数第k个结点，返回结点
	*/
	ListNode* searchLastK(ListNode* l, int k) {
		ListNode* fast = l, * slow = l;
		for (int i = 1; i <= k; i++)fast = fast->next;
		while (fast) {
			fast = fast->next;
			slow = slow->next;
		}
		return slow;
	}
	/**
	 *  数组原地逆置，O(n)
	*/
	void reverse(int* a, int len) {
		int i = 0, j = len - 1;
		while (i < j) {
			swap(a[i++], a[j--]);
		}
	}
	/**
	 *  把数组末尾k到n-1的元素移动到数组开头 0~n-1 -> k~n-1 0~k-1
	 *  算法思想：冒泡法:n方 三次逆置法：O(n)
	*/

	void moveRange(int* a, int len, int k) {
		reverse(a, len);
		reverse(a, len - k);
		reverse(a + len - k, k);
	}
	/**
	 *  相交链表 https://leetcode.cn/problems/intersection-of-two-linked-lists/solution/xiang-jiao-lian-biao-by-leetcode-solutio-a8jn/
	 *  算法思想：短链表指针走到末尾后指向长链表继续走，长链表指针走到末尾后指向短链表继续走，这样到达交点时大家都走了a+b+c步
	*/
	ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
		if (headA == nullptr || headB == nullptr) {
			return nullptr;
		}
		ListNode* pa = headA, * pb = headB;
		while (pa != pb) {
			pa = pa == nullptr ? headB : pa->next;
			pb = pb == nullptr ? headA : pb->next;
		}
		return pa;
	}
	/**
	 * 找到两个有序数组中第k小的数
	*/
	int getKthElement(const vector<int>& nums1, const vector<int>& nums2, int k) {
		/* 主要思路：要找到第 k (k>1) 小的元素，那么就取 pivot1 = nums1[k/2-1] 和 pivot2 = nums2[k/2-1] 进行比较
		 * 这里的 "/" 表示整除
		 * nums1 中小于等于 pivot1 的元素有 nums1[0 .. k/2-2] 共计 k/2-1 个
		 * nums2 中小于等于 pivot2 的元素有 nums2[0 .. k/2-2] 共计 k/2-1 个
		 * 取 pivot = min(pivot1, pivot2)，两个数组中小于等于 pivot 的元素共计不会超过 (k/2-1) + (k/2-1) <= k-2 个
		 * 这样 pivot 本身最大也只能是第 k-1 小的元素
		 * 如果 pivot = pivot1，那么 nums1[0 .. k/2-1] 都不可能是第 k 小的元素。把这些元素全部 "删除"，剩下的作为新的 nums1 数组
		 * 如果 pivot = pivot2，那么 nums2[0 .. k/2-1] 都不可能是第 k 小的元素。把这些元素全部 "删除"，剩下的作为新的 nums2 数组
		 * 由于我们 "删除" 了一些元素（这些元素都比第 k 小的元素要小），因此需要修改 k 的值，减去删除的数的个数
		 */

		int m = nums1.size();
		int n = nums2.size();
		int idx1 = 0, idx2 = 0;

		while (true) {
			// 边界情况
			//1.一个数组已经空了，取另一个数组中的第k大即可
			if (idx1 == m)
				return nums2[idx2 + k - 1];
			if (idx2 == n)
				return nums1[idx1 + k - 1];
			if (k == 1)
				return min(nums1[idx1], nums2[idx2]);

			// 正常情况
			int pivot1 = min(idx1 + k / 2 - 1, m - 1);
			int pivot2 = min(idx2 + k / 2 - 1, n - 1);

			if (nums1[pivot1] <= nums2[pivot2]) {
				k -= pivot1 - idx1 + 1;//删除的元素数
				idx1 = pivot1 + 1;
			}
			else {
				k -= pivot2 - idx2 + 1;
				idx2 = pivot2 + 1;
			}
		}
	}
	/**
	 *  找两个有序数组的中位数 https://leetcode.cn/problems/median-of-two-sorted-arrays/submissions/
	*/
	double findMedianSortedArraysByBsearch(vector<int>& nums1, vector<int>& nums2) {
		int totalLength = nums1.size() + nums2.size();
		if (totalLength % 2 == 1)
			return getKthElement(nums1, nums2, (totalLength + 1) / 2);
		else
			return (getKthElement(nums1, nums2, totalLength / 2) + getKthElement(nums1, nums2, totalLength / 2 + 1)) / 2.0;
	}
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		/**
		 *  中位数有以下性质
		 *  1. 前一部分和后一部分数量相等或者前一部分比后一部分多一个元素
		 *  2. 前一部分的最大值小于后一部分的最小值
		 *  基于性质1，在A、B中各设置一个分点i、j，则i、j把A、B各自划分为两半，A的左半和B的左半归为集合1，A的右半和B的右半归为集合2
		 *  那么由i+j=m+n-i-j或i+j=m+n-i-j+1得j=(m+n+1)/2-i(除法为整数除法)，故可把j消去，二分枚举i求解答案
		 *  基于性质2，所有可能的i必须满足A[i-1]<=B[j]以及B[j-1]<=A[i]，进一步的可推出，只需判断A[i-1]<=B[j]，在所有可能的i中选取最大的i即可，因为
		 *  i为中位数时，此时i最大，i最大时，i+1不满足不等式，有A[i]>B[j-1]
		*/
		if (nums1.size() > nums2.size()) {
			return findMedianSortedArrays(nums2, nums1);
		}

		int m = nums1.size();
		int n = nums2.size();
		int left = 0, right = m;
		// median1：前一部分的最大值
		// median2：后一部分的最小值
		int median1 = 0, median2 = 0;
		//注意这个二分的格式和常规不同
		while (left <= right) {
			// 前一部分包含 nums1[0 .. i-1] 和 nums2[0 .. j-1]
			// 后一部分包含 nums1[i .. m-1] 和 nums2[j .. n-1]
			int i = (left + right) / 2;
			int j = (m + n + 1) / 2 - i;

			// nums_im1, nums_i, nums_jm1, nums_j 分别表示 nums1[i-1], nums1[i], nums2[j-1], nums2[j]
			int nums_i_1 = (i == 0 ? INT_MIN : nums1[i - 1]);
			int nums_i = (i == m ? INT_MAX : nums1[i]);
			int nums_j_1 = (j == 0 ? INT_MIN : nums2[j - 1]);
			int nums_j = (j == n ? INT_MAX : nums2[j]);

			if (nums_i_1 <= nums_j) {
				median1 = max(nums_i_1, nums_j_1);
				median2 = min(nums_i, nums_j);
				left = i + 1;
			}
			else {
				right = i - 1;
			}
		}

		return (m + n) % 2 == 0 ? (median1 + median2) / 2.0 : median1;
	}
	/**
	 *  判断是否是回文链表 https://leetcode.cn/problems/aMhZSa/
	*/
	bool isPalindrome(ListNode* head) {
		if (head == nullptr)return true;
		auto firstHalfTail = middleNodePre(head);
		auto secondHalfHead = reverseList(firstHalfTail->next);
		//判断是否回文
		bool ans = true;
		while (secondHalfHead) {
			if (head->val != secondHalfHead->val) {
				ans = false;
				break;
			}
			head = head->next;
			secondHalfHead = secondHalfHead->next;
		}
		firstHalfTail->next = reverseList(secondHalfHead);
		return ans;
	}
	/**
	 *  找到列表的绝对众数，摩尔投票法 https://leetcode.cn/problems/find-majority-element-lcci/
	*/
	int majorityElement(vector<int>& nums) {
		int candidate = nums[0];
		int count = 1;
		for (int i = 1; i < nums.size(); i++) {
			if (nums[i] == candidate)
				count++;
			else if (count == 1)
				candidate = nums[i];
			else
				count--;
		}
		count = 0;
		for (int& num : nums)
			if (num == candidate)
				count++;
		return count * 2 > nums.size() ? candidate : -1;
	}

}

namespace SkipList {
	constexpr int MAX_LEVEL = 31;
	constexpr double P_FACTOR = 0.25;//2的幂次更快
	template<typename DataType = int>
	struct SkipListNode {
		DataType data;
		vector<SkipListNode*>next;//使用数组有助于cpu缓存
		SkipListNode(DataType data, int level = MAX_LEVEL) :data(data), next(level + 1, nullptr) {}
	};
	template<typename DataType = int, template<typename>typename NodeType = SkipListNode>
	struct SkipList {
		using Node = NodeType<DataType>;
		Node* _head;
		int _level = 0;
		int _size = 0;
		const mt19937 _RANDOM_STANDARD{ random_device{}() };
		uniform_real_distribution<double>_gen;
		SkipList() :_head(new SkipListNode(-1)), _gen(0, 1) {}
		~SkipList() { delete _head; }
		inline int randomLevel() {
			int lv = 0;
			while (_gen(_RANDOM_STANDARD) < P_FACTOR && lv <= MAX_LEVEL)lv++;
			return  lv;
		}
		bool search(DataType target) {
			auto cur = _head;
			//逐层查找到底层
			for (int i = _level; i >= 0; i--)
				while (cur->next[i] && cur->next[i]->data < target)
					cur = cur->next[i];
			cur = cur->next[0];
			if (cur && cur->data == target)return true;
			return false;
		}
		void insert(DataType data) {
			vector<Node*> update(MAX_LEVEL, _head);
			Node* cur = _head;
			for (int i = _level; i >= 0; i--) {
				/* 找到第 i 层小于且最接近 num 的元素*/
				while (cur->next[i] && cur->next[i]->data < data)
					cur = cur->next[i];

				update[i] = cur;
			}
			int lv = randomLevel();
			_level = max(lv, _level);
			auto newNode = new Node(data, lv);
			for (int i = 0; i <= lv; i++) {
				/* 对第 i 层的状态进行更新，将当前元素的 forward 指向新的节点 */
				newNode->next[i] = update[i]->next[i];
				update[i]->next[i] = newNode;
			}
			_size++;
		}
		bool erase(DataType data) {
			vector<Node*>update(MAX_LEVEL, nullptr);
			Node* cur = _head;
			for (int i = _level; i >= 0; i--) {
				/* 找到第 i 层小于且最接近 num 的元素*/
				while (cur->next[i] && cur->next[i]->data < data)
					cur = cur->next[i];
				update[i] = cur;
			}
			cur = cur->next[0];
			/* 如果值不存在则返回 false */
			if (cur == nullptr || cur->data != data)return false;
			for (int i = 0; i <= _level; i++) {
				if (update[i]->next[i] != cur)break;
				/* 对第 i 层的状态进行更新，将 forward 指向被删除节点的下一跳 */
				update[i]->next[i] = cur->next[i];
			}
			delete cur;
			/* 更新当前的 level */
			while (_level > 0 && _head->next[_level] == nullptr)_level--;
			_size--;

			return true;
		}
		Node* kthSmallElement(int k)const {

		}
	};
}

//https://leetcode.cn/problems/all-oone-data-structure/
namespace CountingList {
	//o1时间完成数据的插入、删除，获取最多、最少的元素
	struct Node {
		unordered_set<string>set;
		int cnt;
		Node() {}
		Node(unordered_set<string>&&set, int cnt) :set(move(set)), cnt(cnt) {}
	};
	struct CountingList {
		list<Node>_list;
		unordered_map<string, list<Node>::iterator>_map;
		void inc(string key) {
			if (_map.count(key) > 0) {
				auto cur = _map[key], nxt = next(cur);
				if (nxt == _list.end() || nxt->cnt > cur->cnt + 1)
					_map[key] = _list.emplace(nxt, unordered_set<string>({ key }), cur->cnt + 1);
				else {
					//下个节点的计数刚好比当前节点多一
					nxt->set.insert(key);
					_map[key] = nxt;
				}
				cur->set.erase(key);
				if (cur->set.empty())_list.erase(cur);
			}
			else {
				if (_list.empty() || _list.begin()->cnt > 1)
					_list.emplace_front(unordered_set<string>({ key }), 1);
				else
					_list.begin()->set.emplace(key);
				_map[key] = _list.begin();
			}
		}
		void dec(string key) {
			auto cur = _map[key];
			if (cur->cnt == 1)_map.erase(key);
			else {
				auto pre = prev(cur);
				if (cur == _list.begin() || pre->cnt < cur->cnt - 1)
					_map[key] = _list.emplace(cur, unordered_set<string>({ key }), cur->cnt - 1);
				else {
					//前一个节点计数刚好比当前节点少一
					pre->set.emplace(key);
					_map[key] = pre;
				}
			}
			cur->set.erase(key);
			if (cur->set.empty())_list.erase(cur);
		}
		string getMaxKey() {
			return _list.empty() ? "" : *(_list.rbegin()->set.begin());
		}
		string getMinKey() {
			return _list.empty() ? "" : *(_list.begin()->set.begin());
		}
	};
	
}

//https://leetcode.cn/problems/design-linked-list/submissions/
namespace Deque {
	template<typename DataType=int>
	struct BasicNode {
		DataType data;
		BasicNode* prev, * next;
		BasicNode(){}
		BasicNode(DataType &data):data(data){}
	};
	template<typename DataType=int,template<typename>typename NodeType=BasicNode>
	struct Deque {
		using Node = NodeType<DataType>;
		//dummynode可以简化各项操作
		Node _root;
		Deque(){
			_root.prev = _root.next = &_root;
		}
		~Deque() {
			auto cur = _root.next;
			while (cur != &_root) {
				auto nxt = cur->next;
				delete cur;
				cur = nxt;
			}
		}
		inline Node*& front() { return _root.next; }
		inline Node*& back() { return _root.prev; }
		inline bool empty() { return _root.next = _root.prev; }
		inline void push_back(DataType& data) {
			auto n = new Node(data);
			n->prev = _root.prev;
			_root.prev->next = n;
			_root.prev = n;
			n->next = &_root;
		}
		inline void push_front(DataType& data) {
			auto n = new Node(data);
			n->next = _root.next;
			_root.next->prev = n;
			_root.next = n;
			n->prev = &_root;
		}
		inline void erase(int pos) {
			auto cur = _root.next;
			while (pos--&& cur != &_root) 
				cur = cur->next;
			if (cur != &_root) {
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
				delete cur;
			}
		}
		inline void insertBefore(int pos, DataType& data) {
			auto cur = _root.next;
			while (pos-- && cur != &_root)cur = cur->next;
			auto n = new Node(data);
			if (pos == -1) {
				n->prev = cur->prev;
				cur->prev->next = n;
				n->next = cur;
				cur->prev = n;
			}
		}
		inline Node*& operator[](int pos) {
			auto cur = _root.next;
			while (pos-- > 0 && cur != &_root)
				cur = cur->next;
			return cur;
		}	
	};
}

namespace OrderedMap {
	template<typename KeyType = int, typename ValueType = int>
	struct OrderedMap {
		struct ListNode {
			KeyType k;
			ValueType v;
		};
		list<ListNode>_l;
		unordered_map<KeyType, typename list<ListNode>::iterator>_m;
		inline void push_back(KeyType& k, ValueType& v) {
			_l.push_back({ k,v });
			_m.insert_or_assign(k, _l.back());
		}
		inline void push_front(KeyType& k, ValueType& v) {
			_l.push_front({ k,v });
			_m.insert_or_assign(k, _l.begin());
		}
		inline ListNode& front() { return _l.front(); }
		inline ListNode& back() { return _l.back(); }
		inline typename list<ListNode>::iterator find(KeyType& k) {
			auto p = _m.find(k);
			if (p == _m.end())return _l.end();
			return p->second;
		}
		inline void pop_back() {
			_m.erase(_l.back().k);
			_l.pop_back();
		}
		inline void pop_front() {
			_m.erase(_l.front().k);
			_l.pop_front();
		}
		inline void erase(KeyType& k) {
			auto p = _m.find(k);
			if (p == _m.end())return;
			_l.erase(p->second);
		}
		inline typename list<ListNode>::iterator end() { return _l.end(); }
		inline typename list<ListNode>::iterator begin() { return _l.begin(); }
	};

	//https://leetcode.cn/problems/lru-cache/
	template<typename KeyType = int, typename ValueType = int>
	struct LRUCache :public OrderedMap<KeyType, ValueType> {
		using Supper = OrderedMap<KeyType, ValueType>;
		int _c;
		using Supper::_l;
		using Supper::_m;
		LRUCache(int c) :_c(c) {}
		auto get(KeyType& k) {
			auto p = Supper::find(k);
			if (p == Supper::end())return p;
			_l.splice(_l.begin(), _l, p);
			return p;
		}
		void put(KeyType& k, ValueType& v) {
			auto p = get(k);
			if (p == Supper::end()) {
				if (_l.size() == _c)
					Supper::pop_back();
				Supper::push_front(k, v);
			}
			else p->v = v;
		}
	};

	//https://leetcode.cn/problems/lfu-cache/
	template<typename KeyType = int, typename ValueType = int>
	struct LFUCache {
		struct Node {
			KeyType k;
			ValueType v;
			unsigned int  freq = 1;
			Node() {}
			Node(KeyType& k, ValueType& v) :k(k), v(v) {}
		};
		//key->v key->freq freq->key freq->v
		unordered_map<KeyType, typename list<Node>::iterator>_km;
		unordered_map<unsigned int, list<Node>>_fm;
		unsigned int _c;
		unsigned int _size = 0;
		unsigned int _minfreq = 1;
		LFUCache(int c) :_c(c) {}
		inline auto begin() { return _km.begin(); }
		inline auto end() { return _km.end(); }

		//更新访问频率
		auto get(KeyType& k) {
			auto p = _km.find(k);
			if (p != _km.end()) {
				int & f = p->second->freq;
				auto& l = _fm[f], &newl = _fm[++f];
				newl.splice(newl.begin(), l, p->second);
				if (l.empty()) {
					if (f - 1 == _minfreq)_minfreq++;
					_fm.erase(f - 1);
				}
				p->second = newl.begin();
			}
			return p;
		}
		auto get(KeyType&& k) { return get(k); }
		void put(KeyType& k, ValueType& v) {
			if (_c == 0)return;
			//更新访问频率
			auto p = get(k);
			if (p == _km.end()) {
				//执行缓存淘汰策略 频率相同时执行LRU
				if (_size == _c) {
					auto& s = _fm[_minfreq];
					_km.erase(s.back().k);
					if (s.size() == 1)_fm.erase(_minfreq);
					else s.pop_back();
				}
				else _size++;
				_minfreq = 1;
				//插入 注意要执行LRU
				_fm[1].push_front(Node(k, v));
				_km.insert({ k,_fm[1].begin() });
			}
			else p->second->v = v;
		}
		void put(KeyType&& k, ValueType&& v) { put(k, v); }
	};
}