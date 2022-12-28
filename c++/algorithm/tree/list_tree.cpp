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
		* �ǵݹ��������ģ�� https://leetcode.cn/problems/kTOapQ/solution/er-cha-sou-suo-shu-die-dai-qi-by-leetcod-hwfe/
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
		//�������� �����
		void lRotate(Node*& cur) {
			Node* r = cur->r;
			cur->r = r->l;
			r->l = cur;
			cur = r;
		}
		//�������� �ҹ���
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
		* �ж����Ƿ�ƽ��  https://leetcode.cn/problems/balanced-binary-tree/submissions/
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
				//ǰ������ĵ�һ���ڵ��Ǹ��ڵ�
				DataType val = preorder[pl];
				//ͨ���������ȷ���������Ĵ�С���������Ĵ�С
				int irIdx = idx[val];
				int lsize = irIdx - il;
				//�½����ڵ�
				auto root = new Node(val);
				//����ǰ����� �� ������ ������ �ݹ黮�������������Ը��ڵ���ӽڵ��������
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
				//һֱ�����ߣ�û�ߵ������½���һֱ���ӵ�ǰ�ڵ�Ϊ��һ���ڵ�����ӽڵ㣬������ǰ�ڵ���ջ��
				if (n->data != inorder[irIdx]) {
					n->l = new Node(val);
					s.push(n->l);
				}
				//�ߵ�������ߣ���ʼ�����ӽڵ�
				else {
					//ע�����ߵ���һ�����ӽڵ�֮ǰ��ǰ�������·�������������·�����෴�ģ���������ִ�е�ջ��ֱ��ջ��Ԫ�غ����������Ԫ�ز�ͬ����ʱ�õ��ľ������ӽڵ㡣
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
			//�ŵ�ָ��ַ���
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
		* �ж϶������Ƿ���Գ� https://leetcode.cn/problems/dui-cheng-de-er-cha-shu-lcof/
		*/
		bool isSymmetric() {
			return  _root == nullptr || isSymmetric(_root->l, _root->r);
		}
		bool isSymmetric(Node* A, Node* B) {
			return (A == nullptr && B == nullptr || A && B && A->data == B->data && isSymmetric(A->l, B->r) && isSymmetric(A->l, B->r));//����Ҫ������
		}
		/**
		* �ж϶������Ƿ����
		*/
		bool same(Node* A, Node* B) {
			return A == nullptr && B == nullptr || A && B && A->data == B->data && same(A->l, B->l) && same(A->r, B->r);
		}
		/**
		* �ж�A�Ƿ���B������ https://leetcode.cn/problems/subtree-of-another-tree/submissions/
		*/
		bool isSubtree(Node* subRoot, Node* root) {
			if (!root && subRoot)return false;
			//same�ǵݹ�ģ������󣬿����ж�
			return same(root, subRoot) || isSubtree(root->l, subRoot) || isSubtree(root->r, subRoot);
		}
		/**
		* ��ת������ https://leetcode.cn/problems/er-cha-shu-de-jing-xiang-lcof/
		*/
		Node* flip(Node* A, Node* B = nullptr) {
			if (A == nullptr)return nullptr;
			B = new Node(A->data);
			B->l = flip(A->r, B->l);
			B->r = flip(A->l, B->r);
			return B;
		}
		/**
		* ԭ�ط�ת������ https://leetcode.cn/problems/invert-binary-tree/
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
		* չƽ������ https://leetcode.cn/problems/NYBBNL/
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
		* �������Ҳ�ڵ� https://leetcode.cn/problems/WNC0Lk/
		*/
		vector<DataType> rightView(Node* root) {
			vector<DataType>ans;
			if (root == nullptr)return ans;
			queue<Node*>q;
			q.push(root);
			while (!q.empty()) {
				Node* n;
				//�������ģ��
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
		* ��������ֱ�� ����dp d=max{max lh + max rh +1} https://leetcode.cn/problems/diameter-of-binary-tree/
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
			return ans - 1;//ֱ���Ǳߵĳ��Ⱥ�
		}
		/**
		* ����������� ���ʣ�ֻ������������ȿ��������������зֱ����p��q https://leetcode.cn/problems/er-cha-shu-de-zui-jin-gong-gong-zu-xian-lcof/
		*/
		Node* lowestCommonAncestor(Node* root, Node* p, Node* q) {
			if (root == nullptr || p == root || q == root)return root;
			Node* l = lowestCommonAncestor(root->l, p, q);
			Node* r = lowestCommonAncestor(root->r, p, q);
			return l == nullptr ? r : (r == nullptr ? l : root);
		}
		/**
		* ������ת˫��ѭ������ leecode�޸İ� https://leetcode.cn/problems/er-cha-sou-suo-shu-yu-shuang-xiang-lian-biao-lcof/submissions/
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
		* У������� https://leetcode.cn/problems/validate-binary-search-tree/
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
		* ��ת�ȼ� https://leetcode.cn/problems/flip-equivalent-binary-trees/submissions/
		*/

		bool flipEquiv(Node* root1, Node* root2) {
			//���Զ�����ȷ�϶������������ʵĻ����Ե����Ϸ�ת
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
		     * ������������� �����þͲ���Ҫ���ڵ㣬ֱ�ӿ����޸ĸ��ڵ�ĺ���ָ��
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
				//��������
				if (f > 1) {
					if (factor(cur->l) >= 0)LL(cur);
					else LR(cur);
				}
				//��������
				else if(f<-1){
					if (factor(cur->r) <= 0)RR(cur);
					else RL(cur);
				}
				else update(cur);//���������cur���θ���
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
						//���������ڵ�
						if (curA->data > curB->data) {
							swap(curA->data, curB->data);
							swap(curA->child, curB->child);
						}
						//�ϲ�
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
						//�ϲ�
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
				//�ڶ������ɾ����Ӧ�Ķ�����
				*p = top->sibling;
				//�ϲ�ɾ��������ӶѺ�ԭ��
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
			//����������ܵ��Կ죬��ȫ���������,Ҳ���õ����ڴ�й¶
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
				//2��3 �� 1�ϲ� 2�ϲ���Ϊ1�ĺ���
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
			// ���ȵݹ��
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
	* �߶��� ����lognʱ�������������� ��ǰ׺�͡������O(n))
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
					//����ʱ���¸��ڵ�
					n->sum = n->ls->sum + n->rs->sum;
				}
			}
			void add(T delta, int l, int r, int curl, int curr, Node* n) {
				if (curl > r || curr < l)return;
				//ȫ����
				if (l <= curl && curr <= r) {
					n->sum += (curr - curl + 1) * delta;
					//��Ҷ��
					if (curr != curl)n->mark += delta;
				}
				//���ְ���
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
			//����assign downLazyTagҪ�������
			void downLazyTag(Node* n, int len) {
				n->ls->mark += n->mark;
				n->rs->mark += n->mark;
				n->ls->sum += n->mark * (len - len / 2);
				n->rs->sum += n->mark * (len / 2);//�ұ߶�һ��
				n->mark = 0;
			}
		};
	}

	namespace Heap {
		struct Heap {
			vector<int>_h;//�±��0��ʼ
			//�ڵ��³�
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
			//�ڵ�����
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
