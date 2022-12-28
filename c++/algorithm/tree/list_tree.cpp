#include <concepts>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <list>
using namespace std;

namespace ListTree {
	template <typename DataType = int>
	struct TreeNode {
		DataType data;
		TreeNode* l = nullptr, * r = nullptr;
		TreeNode() {}
		TreeNode(DataType&& data) :data(move(data)) {}
		TreeNode(DataType& data) :data(data) {}
		~TreeNode() { delete l; delete r; }
	};

	template <typename DataType = int, template<typename>typename NodeType = TreeNode>
		requires derived_from<NodeType<DataType>, TreeNode<DataType>>
	struct Tree {
		using Node = NodeType<DataType>;
		/**
		* 非递归中序遍历模板 https://leetcode.cn/problems/kTOapQ/solution/er-cha-sou-suo-shu-die-dai-qi-by-leetcod-hwfe/
		*/
		struct BSTInorderIterator {
			Node* cur;
			stack<Node*>s;
			BSTInorderIterator(Node* root) :cur(root) {}
			DataType next() {
				while (cur != nullptr) {
					s.push(cur);
					cur = cur->l;
				}
				cur = s.top(); s.pop();
				DataType ans = cur->data;
				cur = cur->r;
				return ans;
			}
			inline bool hasNext() { return cur != nullptr || !s.empty(); }
		};
		Node* _root;
		Tree() {}
		~Tree() { delete _root; }
		//左旋拎右 左挂右
		void lRotate(Node*& cur) {
			Node* r = cur->r;
			cur->r = r->l;
			r->l = cur;
			cur = r;
		}
		//右旋拎左 右挂左
		void rRotate(Node*& cur) {
			Node* l = cur->l;
			cur->l = l->r;
			l->r = cur;
			cur = l;
		}
		inline void LL(Node*& cur) { rRotate(cur); }
		inline void LR(Node*& cur) { lRotate(cur); rRotate(cur); }
		inline void RL(Node*& cur) { rRotate(cur); lRotate(cur); }
		inline void RR(Node*& cur) { lRotate(cur); }
		template<typename U = DataType>
		Node* successor(U&& data) {
			Node* ans = nullptr;
			Node* cur = _root;
			while (cur) {
				if (cur->data > data) {
					ans = cur;
					cur = cur->l;
				}
				else cur = cur->r;
			}
			return ans;
		}
		template<typename U = DataType>
		Node* precursor(U&& data) {
			Node* ans = nullptr;
			Node* cur = _root;
			while (cur) {
				if (cur->data < data) {
					ans = cur;
					cur = cur->r;
				}
				else cur = cur->l;
			}
			return ans;
		}
		Node* parent(Node* n) {
			if (n == _root)return nullptr;
			Node* ans = _root;
			while (ans->l != n && ans->r != n) {
				if (ans->data < n->data)ans = ans->r;
				else ans = ans->l;
			}
			return ans;
		}
		template<typename U = DataType>
		Node* find(Node* cur, U&& data) {
			if (cur == nullptr)return nullptr;
			if (data == cur->data)return cur;
			if (data < cur->data)return find(cur->l, data);
			return find(cur->r, data);
		}
		/**
		* 判断树是否平衡  https://leetcode.cn/problems/balanced-binary-tree/submissions/
		*/
		bool isBalanced(Node* root) {
			auto dfs = [&](auto&& self, Node* cur)->int {
				if (cur == nullptr)return 0;
				int lh = self(self, cur->l);
				int rh = self(self, cur->r);
				if (lh == -1 || rh == -1 || abs(lh - rh) > 1)return -1;
				return max(lh, rh) + 1;
			};
			return dfs(dfs, root) == -1 ? false : true;
		}
		//https://leetcode.cn/problems/zhong-jian-er-cha-shu-lcof/
		Node* buildByPreorderAndInorderRecursive(DataType preorder[], DataType inorder[], int pLen, int iLen) {
			unordered_map<int, int>idx;
			for (int i = 0; i < pLen; i++)idx[inorder[i]] = i;
			auto dfs = [&](auto&& self, int preorder[], int inorder[], int pl, int pr, int il, int ir)->Node* {
				if (pl > pr)return nullptr;
				//前序遍历的第一个节点是根节点
				DataType val = preorder[pl];
				//通过中序遍历确定左子树的大小和右子树的大小
				int irIdx = idx[val];
				int lsize = irIdx - il;
				//新建根节点
				auto root = new Node(val);
				//根据前序遍历 根 左子树 右子树 递归划分左右子树并对根节点和子节点进行连接
				root->l = self(self, preorder, inorder, pl + 1, pl + lsize, il, irIdx - 1);
				root->r = self(self, preorder, inorder, pl + lsize + 1, pr, irIdx + 1, ir);
				return root;
			};
			return dfs(dfs, preorder, inorder, 0, pLen - 1, 0, iLen - 1);
		}
		Node* buildByPreorderAndInorder(DataType preorder[], DataType inorder[], int pLen, int iLen) {
			if (!pLen)return nullptr;
			auto root = new Node(preorder[0]);
			stack<Node*> s;
			s.push(root);
			for (int i = 1, irIdx = 0; i < pLen; i++) {
				DataType val = preorder[i];
				auto n = s.top();
				//一直向左走，没走到最左下角则一直连接当前节点为上一个节点的左子节点，并将当前节点入栈。
				if (n->data != inorder[irIdx]) {
					n->l = new Node(val);
					s.push(n->l);
				}
				//走到了最左边，开始找右子节点
				else {
					//注意在走到第一个右子节点之前，前序遍历的路径和中序遍历的路径是相反的，所以我们执行弹栈，直到栈顶元素和中序遍历的元素不同，这时得到的就是右子节点。
					while (!s.empty() && s.top()->data == inorder[irIdx]) {
						n = s.top(); s.pop();
						++irIdx;
					}
					n->r = new Node(val);
					s.push(n->r);
				}
			}
			return root;
		}
		struct dataToStr {
			inline string operator()(DataType& data) {
				return to_string(data);
			}
		};
		template <typename Func = dataToStr>
		string serializeByLeetCodeFormat(Func dataToStr = Func()) {
			if (_root == nullptr)return "[]"s;
			queue<Node*>q;
			q.push(_root);
			vector<Node*>a;
			while (!q.empty()) {
				auto p = q.front(); q.pop();
				if (p)q.push(p->l), q.push(p->r);
				a.push_back(p);
			}
			for (int i = a.size() - 1; a[i] == nullptr; i--)a.pop_back();
			string ans = "[";
			for (auto& p : a) {
				if (p)ans += dataToStr(p->data) + ","s;
				else ans += "null,";
			}
			ans.back() = ']';
			return ans;
		}
		struct strToData {
			inline int operator()(string& s) {
				return stoi(s);
			}
		};
		template<typename Func = strToData>
		Node* deserializeByLeetCodeFormat(string& s, Func strToData = Func()) {
			if (s.size() <= 2)return nullptr;
			s.pop_back();
			s.erase(0, 1);
			//古典分割字符串
			stringstream ss(s);
			string part;
			vector<string>a;
			while (getline(ss, part, ','))
				a.push_back(part);
			queue<Node*>q;
			auto root = new Node(strToData(a[0]));
			q.push(root);
			for (int i = 1; i < a.size(); i++) {
				auto n = q.front(); q.pop();
				if (a[i] != "null") {
					n->l = new Node(strToData(a[i]));
					q.push(n->l);
				}
				i++;
				if (i < a.size() && a[i] != "null") {
					n->r = new Node(strToData(a[i]));
					q.push(n->r);
				}
			}
			return root;
		}
		/*
		* 判断二叉树是否镜像对称 https://leetcode.cn/problems/dui-cheng-de-er-cha-shu-lcof/
		*/
		bool isSymmetric() {
			return  _root == nullptr || isSymmetric(_root->l, _root->r);
		}
		bool isSymmetric(Node* A, Node* B) {
			return (A == nullptr && B == nullptr || A && B && A->data == B->data && isSymmetric(A->l, B->r) && isSymmetric(A->l, B->r));//镜像要反着来
		}
		/**
		* 判断二叉树是否相等
		*/
		bool same(Node* A, Node* B) {
			return A == nullptr && B == nullptr || A && B && A->data == B->data && same(A->l, B->l) && same(A->r, B->r);
		}
		/**
		* 判断A是否是B的子树 https://leetcode.cn/problems/subtree-of-another-tree/submissions/
		*/
		bool isSubtree(Node* subRoot, Node* root) {
			if (!root && subRoot)return false;
			//same是递归的，开销大，靠后判断
			return same(root, subRoot) || isSubtree(root->l, subRoot) || isSubtree(root->r, subRoot);
		}
		/**
		* 翻转二叉树 https://leetcode.cn/problems/er-cha-shu-de-jing-xiang-lcof/
		*/
		Node* flip(Node* A, Node* B = nullptr) {
			if (A == nullptr)return nullptr;
			B = new Node(A->data);
			B->l = flip(A->r, B->l);
			B->r = flip(A->l, B->r);
			return B;
		}
		/**
		* 原地翻转二叉树 https://leetcode.cn/problems/invert-binary-tree/
		*/
		Node* flipInplace(Node* root) {
			if (root == nullptr)return nullptr;
			auto l = flipInplace(root->r);
			auto r = flipInplace(root->l);
			root->l = r;
			root->r = l;
			return root;
		}
		/**
		* 展平二叉树 https://leetcode.cn/problems/NYBBNL/
		*/
		Node* flatRecursive(Node* root) {
			auto dummyNode = Node(-1);
			auto lastNode = &dummyNode;
			auto dfs = [&](auto&& self, Node* cur) {
				if (cur == nullptr)return;
				self(self, cur->l);
				lastNode->r = cur;
				cur->l = nullptr;
				lastNode = cur;
				self(self, cur->r);
			};
			dfs(dfs, root);
			return dummyNode.r;
		}
		Node* flat(Node* root) {
			stack<Node*>s;
			Node dummmyNode = TreeNode(-1);
			Node* pre = &dummmyNode;
			while (root || !s.empty()) {
				while (root) {
					s.push(root);
					root = root->l;
				}
				root = s.top(); s.pop();
				root->l = nullptr;
				pre->r = root;
				pre = root;
				root = root->r;
			}
			return dummmyNode.r;
		}
		/**
		* 二叉树右侧节点 https://leetcode.cn/problems/WNC0Lk/
		*/
		vector<DataType> rightView(Node* root) {
			vector<DataType>ans;
			if (root == nullptr)return ans;
			queue<Node*>q;
			q.push(root);
			while (!q.empty()) {
				Node* n;
				//层序遍历模板
				for (int i = 0, size = q.size(); i < size; i++) {
					n = q.front(); q.pop();
					if (n->l)
						q.push(n->l);
					if (n->r)
						q.push(n->r);
				}
				ans.push_back(n->data);
			}
			return ans;
		}
		/**
		* 二叉树的直径 树形dp d=max{max lh + max rh +1} https://leetcode.cn/problems/diameter-of-binary-tree/
		*/
		int diameter(Node* root) {
			int ans = 0;
			auto dfs = [&](auto&& self, Node* root)->int {
				if (root == nullptr)return 0;
				int lh = self(self, root->l);
				int rh = self(self, root->r);
				ans = max(ans, lh + rh + 1);
				return max(lh, rh) + 1;
			};
			return ans - 1;//直径是边的长度和
		}
		/**
		* 最近公共祖先 性质：只有最近公共祖先可能在左右子树中分别包含p、q https://leetcode.cn/problems/er-cha-shu-de-zui-jin-gong-gong-zu-xian-lcof/
		*/
		Node* lowestCommonAncestor(Node* root, Node* p, Node* q) {
			if (root == nullptr || p == root || q == root)return root;
			Node* l = lowestCommonAncestor(root->l, p, q);
			Node* r = lowestCommonAncestor(root->r, p, q);
			return l == nullptr ? r : (r == nullptr ? l : root);
		}
		/**
		* 二叉树转双向不循环链表 leecode修改版 https://leetcode.cn/problems/er-cha-sou-suo-shu-yu-shuang-xiang-lian-biao-lcof/submissions/
		*/
		Node* convertToSortedDoubleList(Node* root) {
			if (root == nullptr)return nullptr;
			Node dummyNode(-1);
			Node* lastNode = &dummyNode;
			auto dfs = [&](auto&& self, Node* cur) {
				if (cur == nullptr)return;
				self(self, cur->l);
				cur->l = lastNode;
				lastNode->r = cur;
				lastNode = cur;
				self(self, cur->r);
			};
			dfs(dfs, root);
			dummyNode.r->l = nullptr;
			return dummyNode.right;
		}
		/**
		* 校验二叉树 https://leetcode.cn/problems/validate-binary-search-tree/
		*/
		bool isValidBST(Node* root) {
			if (root == nullptr || root->l == nullptr && root->r == nullptr)return true;
			auto dfs = [&](auto&& self, TreeNode* cur, long long upper, long long lower)->bool {
				if (cur == nullptr)return true;
				if (cur->val <= lower || cur->val >= upper)return false;
				return self(self, cur->l, cur->val, lower) && self(self, cur->r, upper, cur->val);
			};
			return dfs(dfs, root, LONG_MAX, -LONG_MAX);
		}
		/**
		* 翻转等价 https://leetcode.cn/problems/flip-equivalent-binary-trees/submissions/
		*/

		bool flipEquiv(Node* root1, Node* root2) {
			//先自顶向下确认二叉树，不合适的话再自底向上翻转
			return root1 == nullptr && root2 == nullptr || root1 && root2 && root1->val == root2->val && (flipEquiv(root1->l, root2->l) && flipEquiv(root1->r, root2->r) || flipEquiv(root1->l, root2->r) && flipEquiv(root1->r, root2->l));
		}
	};


	namespace AVLTree {
		struct Node {
			int val, height;
			Node* l, * r;
			Node(int val) :val(val),height(0),l(nullptr),r(nullptr){}
		};
		struct AVL {
			Node* _root = nullptr;
			/**
		     * 左旋拎右左挂右 传引用就不需要父节点，直接可以修改父节点的孩子指针
		    */
			void lRotate(Node* &cur) {
				auto r = cur->r;
				cur->r = cur->r->l;
				cur->r->l = cur;
				cur = r;
				update(cur->l);
				update(cur);
			}
			void rRotate(Node* &cur) {
				auto l = cur->l;
				cur->l = cur->l->r;
				cur->l->r = cur;
				cur = l;
				update(cur->r);
				update(cur);
			}
			void LL(Node*&cur) {
				rRotate(cur);
			}
			void LR(Node*& cur) {
				lRotate(cur->l);
				rRotate(cur);
			}
			void RR(Node*& cur) {
				lRotate(cur);
			}
			void RL(Node*& cur) {
				rRotate(cur->r);
				lRotate(cur);
			}
			void update(Node* cur) {
				cur->height = max(cur->l->height, cur->r->height) + 1;
			}
			int factor(Node* cur) { return cur->l->height - cur->r->height; }
			void balance(Node*& cur) {
				if (cur == nullptr)return;
				int f = factor(cur);
				//左子树高
				if (f > 1) {
					if (factor(cur->l) >= 0)LL(cur);
					else LR(cur);
				}
				//右子树高
				else if(f<-1){
					if (factor(cur->r) <= 0)RR(cur);
					else RL(cur);
				}
				else update(cur);//其他情况的cur会多次更新
			}
			void insert(Node*&cur,int data) {
				if (cur == nullptr) {
					cur = new Node(data);
					return;
				}
				else if (data < cur->val)insert(cur->l, data);
				else if (data > cur->val)insert(cur->r, data);
				balance(cur);
			}
		};
		
	}

	namespace BinomialHeap {

		template<typename DataType = int>
		struct Node {
			DataType data;
			int degree = 0;
			Node* child = nullptr, * sibling = nullptr;
			Node() {}
			Node(DataType data) :data(data) {}
		};
		template<typename DataType = int, template<typename>typename NodeType = Node>
		struct BinomialHeap {
			using Node = NodeType<DataType>;
			Node* _head = nullptr;
			Node** _top = nullptr;
			inline Node* newNode(DataType data) { return new Node(data); }
			//merge b into a
			Node* mergeHeap(Node* a, Node* b) {
				Node** pA = &a, ** pB = &b, * curA, * curB;
				//0 1 01 11 001
				while ((curA = *pA) != nullptr && (curB = *pB) != nullptr) {
					if (curA->data < (*_top)->data)_top = pA;

					if (curA->degree < curB->degree)
						pA = &(curA->sibling);
					else if (curA->degree == curB->degree) {
						pB = &(curB->sibling);
						//交换两个节点
						if (curA->data > curB->data) {
							swap(curA->data, curB->data);
							swap(curA->child, curB->child);
						}
						//合并
						curB->sibling = curA->child;
						curA->child = curB;
						curA->degree++;
					}
					else {
						*pB = curB->sibling;
						*pA = curB;
						curB->sibling = curA;
						pA = &(curB->sibling);
					}
				}
				while (curA) {
					auto nxt = curA->sibling;
					if (nxt && curA->degree == nxt->degree) {
						if (curA->data > nxt->data) {
							swap(curA->data, nxt->data);
							swap(curA->child, nxt->child);
						}
						//合并
						curA->sibling = nxt->sibling;
						nxt->sibling = curA->child;
						curA->child = nxt;
						curA->degree++;
						if (curA->data < (*_top)->data)_top = pA;
					}
				}
				if (curB)*pA = curB;
				return a;
			}
			inline void insert(DataType data) {
				auto n = newNode(data);
				_head = mergeHeap(_head, n);
			}
			inline Node* top() { return *_top; }
			void pop() {
				if (_head == nullptr)return;
				auto p = _top;
				auto top = *p;
				//在二项堆中删除对应的二项树
				*p = top->sibling;
				//合并删除根后的子堆和原堆
				_head = mergeHeap(_head, top->child);
			}
		};
	}
	//https://www.cnblogs.com/Paul-Guderian/p/7664366.html
	//https://github.com/saadtaame/pairing-heap/blob/master/pairing_heap.cc
	//https://www.geeksforgeeks.org/pairing-heap/
	//https://people.ksp.sk/~kuko/gnarley-trees/Pairing.html
	//https://zh.wikipedia.org/wiki/%E4%BA%8C%E9%A1%B9%E5%A0%86#%E6%9F%A5%E6%89%BE%E6%9C%80%E5%B0%8F%E5%85%B3%E9%94%AE%E5%AD%97%E6%89%80%E5%9C%A8%E7%BB%93%E7%82%B9
	namespace PairingHeap {
		template<typename DataType = int>
		struct BasicNode {
			DataType data;
			BasicNode* child, * sibling;
			BasicNode() {}
			BasicNode(DataType&& data) :data(move(data)) {}
			BasicNode(DataType& data) :data(data) {}
		};
		template<typename DataType = int, template<typename>typename NodeType = BasicNode>
		struct PairingHeap {
			using Node = NodeType<DataType>;
			//用数组可以跑得稍快，完全媲美二叉堆,也不用担心内存泄露
			Node* _h = nullptr;
			Node* _root = nullptr;
			int cnt = 0;
			list<Node*>recycle;
			PairingHeap(int c) :_h(new Node[c]) {}
			~PairingHeap() { delete _h; }
			inline bool empty() { return cnt == 0; }
			inline Node*& top() { return _root; }
			inline Node* newNode(DataType& data) {
				Node* ans;
				if (recycle.empty())ans = &_h[++cnt];
				else { ans = recycle.back(); recycle.pop_back(); }
				ans->child = ans->sibling = nullptr;
				ans->data = data;
				return ans;
			}
			inline Node* merge(Node* a, Node* b) {
				if (a == nullptr)return b;
				if (b == nullptr)return a;
				if (a->data >= b->data)swap(a, b);
				//2、3 和 1合并 2合并作为1的孩子
				if (a->child != nullptr)b->sibling = a->child;
				a->child = b;
				return a;
			}
			inline void insert(DataType& data) { _root = merge(_root, newNode(data)); }
			Node* mergeChildren(Node* cur) {
				if (cur == nullptr || cur->sibling == nullptr)return cur;
				Node* a = cur->sibling, * b = a->sibling;
				cur->sibling = a->sibling = nullptr;
				return merge(merge(cur, a), mergeChildren(b));
			}
			inline void pop() {
				if (_root == nullptr)return;
				recycle.push_back(_root);
				_root = mergeChildren(_root->child);
			}
			// 不比递归快
			inline void _pop() {
				if (_root == nullptr)return;
				recycle.push_back(_root);
				if (_root->child == nullptr) { _root = nullptr; return; }
				list<Node*>l;
				auto cur = _root->child;
				while (cur) {
					auto sibling = cur->sibling;
					if (sibling == nullptr) {
						l.push_back(cur);
						break;
					}
					auto nxt = sibling->sibling;
					cur->sibling = sibling->sibling = nullptr;
					l.push_back(merge(cur, sibling));
					cur = nxt;
				}
				_root = nullptr;
				for (auto& n : l)_root = merge(_root, n);
			}
		};
	}

	/**
	* 线段树 用于logn时间内完成区间操作 （前缀和、差分是O(n))
	*/
	namespace SegmentTree {
		template<typename T = int>
		struct BasicNode {
			T sum, mark;
			BasicNode* ls, * rs;
		};
		template<typename T = int>
		struct SegmentTree {
			using Node = BasicNode<T>;
			T* _a;
			int _size;
			int _cnt = 0;
			Node* _root;
			SegmentTree(T* a, int size) :_a(a), _size(size) {
				_root = new Node[_size * 2]();
				build(0, _size - 1, _root);
			}
			void build(int l, int r, Node*& n) {
				if (n == nullptr)n = &_root[++_cnt];
				if (l == r)n->sum = _a[l];
				else {
					int mid = (l + r) >> 1;
					build(l, mid, n->ls);
					build(mid + 1, r, n->rs);
					//回溯时更新根节点
					n->sum = n->ls->sum + n->rs->sum;
				}
			}
			void add(T delta, int l, int r, int curl, int curr, Node* n) {
				if (curl > r || curr < l)return;
				//全包含
				if (l <= curl && curr <= r) {
					n->sum += (curr - curl + 1) * delta;
					//非叶子
					if (curr != curl)n->mark += delta;
				}
				//部分包含
				else {
					downLazyTag(n, curr - curl + 1);
					int mid = (curl + curr) >> 1;
					add(delta, l, r, curl, mid, n->ls);
					//mid+1<=r
					add(delta, l, r, mid + 1, curr, n->rs);
					n->sum = n->ls->sum + n->rs->sum;
				}
			}
			void add(T delta, int l, int r) {
				add(delta, l, r, 0, _size - 1, _root);
			}
			T query(int l, int r, int curl, int curr, Node* n) {
				if (curl > r || curr < l)return 0;
				if (l <= curl && curr <= r)return n->sum;
				int mid = (curl + curr) >> 1;
				downLazyTag(n, curr - curl + 1);
				return query(l, r, curl, mid, n->ls) + query(l, r, mid + 1, curr, n->rs);

			}
			T query(int l, int r) { return query(l, r, 0, _size - 1, _root); }
			//对于assign downLazyTag要重新设计
			void downLazyTag(Node* n, int len) {
				n->ls->mark += n->mark;
				n->rs->mark += n->mark;
				n->ls->sum += n->mark * (len - len / 2);
				n->rs->sum += n->mark * (len / 2);//右边短一点
				n->mark = 0;
			}
		};
	}

	namespace Heap {
		struct Heap {
			vector<int>_h;//下标从0开始
			//节点下沉
			void siftDown(int i) {
				while (i < _h.size()) {
					int l = i * 2 + 1, r = i * 2 + 2;
					int large = i;
					if (l<_h.size() && _h[l]>_h[large])large = l;
					if (r<_h.size() && _h[r]>_h[large])large = r;
					if (large != i) {
						swap(_h[i], _h[large]);
						i = large;
					}
					else break;
				}
			}
			//节点上升
			void siftUp(int i) {
				while (i) {
					if (_h[(i - 1) / 2] > _h[i]) {
						swap(_h[(i - 1) / 2], _h[i]);
						i = (i - 1) / 2;
					}
					else break;
				}
			}
			void push(int val) {
				_h.push_back(val);
				siftUp(_h.size() - 1);
			}
			int top() {
				return _h.front();
			}
			void pop() {
				swap(_h.front(), _h.back());
				_h.pop_back();
				siftDown(0);
			}
		};
	}
}
