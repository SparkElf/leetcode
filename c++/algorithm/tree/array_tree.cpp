#include <iostream>
#include <vector>
#include <stack>
#include <concepts>
#include <cstring>
#include <unordered_map>
#include <utility>
#include <stack>

using namespace std;

namespace ArrayTree {
	template <typename DataType=int>
	struct TreeNode {
		DataType data;
		int l, r;
	};
	template <typename DataType=int, template<typename> typename NodeType = TreeNode>
		requires derived_from<NodeType<DataType>, TreeNode<DataType>>
	struct Tree {
		using Node = NodeType<DataType>;
		Node* t;//首元节点从1开始		
		int root = 0;//root是会变的，不是常量
		int cnt = 0;
		int c;
		Node& operator [](int i) {return t[i];}
		template <typename U=DataType>
		int newNode(U&& data) {
			t[++cnt].data = data;
			t[cnt].l = t[cnt].r = 0;
			return cnt;
		}
		/**
		* 左旋拎右左挂右 传引用就不需要父节点，直接可以修改父节点的孩子指针
		*/
		void lRotate(int& cur) {
			int r = t[cur].r;//右子树
			t[cur].r = t[r].l;//左挂右
			t[r].l = cur;//拎右
			cur = r;
		}
		/**
		 *  右旋拎左右挂左
		*/
		void rRotate(int& cur) {
			int l = t[cur].l;
			t[cur].l = t[l].r;
			t[l].r = cur;
			cur = l;
		}
		/**
		 *  左直线型，左子树高，且左子树的左子树高 直接对根节点右旋
		*/
		inline void LL(int& cur) {
			rRotate(cur);
		}
		/**
		*  左Z型，左子树高，且左子树的右子树高 左旋左子树变成LL，右旋自己
		*/
		inline void LR(int& cur) {
			lRotate(t[cur].l);
			rRotate(cur);
		}
		/**
		*  右Z型，右子树高，且右子树的左子树高 右旋右子树，再左旋自己
		*/
		inline void RL(int& cur) {
			rRotate(t[cur].r);
			lRotate(cur);
		}
		/**
		*  右直线型，右子树高，且右子树的右子树高 直接对根节点左旋
		*/
		inline void RR(int& cur) {
			lRotate(cur);
		}
		/**https://leetcode.cn/problems/P5rCT8/solution/er-cha-sou-suo-shu-zhong-de-zhong-xu-hou-5nrz/
		 * 情况一：
		 * 二叉搜索树满足左子树小于根节点小于右子树，所以左子树最右边的节点是小于根节点的最大的元素，是根节点的前驱，
		 * 右子树最左边的节点是大于根节点的最小的元素，所以是根节点的后继。
		 * 情况二：
		 * 节点不存在右子树时，则需要从头开始遍历定位才能找到后继
		 * 节点不存在左子树时，则需要从头开始遍历定位才能找到前驱
		 * return:successor
		*/
		int successor(int cur) {
			int ans = 0;
			if (t[cur].r) {
				ans = t[cur].r;
				while (t[cur].l)
					ans = t[cur].l;
				return ans;
			}
			int n = root;
			while (n) {
				if (t[n].data > t[cur].data) {
					ans = n;
					n = t[n].l;
				}
				else n = t[n].r;
			}
			return ans;
		}
		int precursor(int cur) {
			int ans = 0;
			if (t[cur].l) {
				ans = t[cur].l;
				while (t[cur].r)
					ans = t[cur].r;
				return ans;
			}
			int n = root;
			while (n) {
				if (t[n].data < t[cur].data) {
					ans = n;
					n = t[n].r;
				}
				else n = t[n].l;
			}
			return ans;
		}
		int parent(int cur) {
			if (cur == 1)return 0;
			int ans = 1;
			while (t[ans].l != cur && t[ans].r != cur) {
				if (t[ans].data < t[cur].data)ans = t[ans].r;
				else ans = t[ans].l;
			}
			return ans;
		}
		int find(int cur, DataType&& data) {
			if (cur == 0)return 0;
			if (t[cur].data == data)return cur;
			if (data < t[cur].data)return find( t[cur].l, move(data));
			return find(t[cur].r, move(data));
		}
		/**
		* 从中序遍历和前序遍历构建二叉树，返回根节点 
		* https://leetcode.cn/problems/zhong-jian-er-cha-shu-lcof/solution/mian-shi-ti-07-zhong-jian-er-cha-shu-by-leetcode-s/
		*/
		int buildByPreorderAndInorderRecursive(int preorder[], int inorder[], int pLen,int iLen,Tree* _this=this) {
			unordered_map<int, int>idx;
			for (int i = 0; i < pLen; i++)idx[inorder[i]] = i;
			auto dfs = [&](auto&&self, int preorder[], int inorder[], int pl,int pr,int il,int ir)->int {
				if (pl>pr)return 0;
				//前序遍历的第一个节点是根节点
				DataType val = preorder[pl];
				//通过中序遍历确定左子树的大小和右子树的大小
				int irIdx = idx[val];
				int lsize = irIdx - il;
				//新建根节点
				int root = _this->newNode(val);
				//根据前序遍历 根 左子树 右子树 递归划分左右子树并对根节点和子节点进行连接
				t[root].l = self(self, preorder, inorder, pl + 1, pl + lsize, il, irIdx - 1);
				t[root].r = self(self, preorder, inorder,pl+lsize+1,pr,irIdx+1,ir);
				return root;
			};
			return dfs(dfs, preorder, inorder, 0,pLen-1, 0,iLen-1);
		}
		int buildByPreorderAndInorder(DataType preorder[], DataType inorder[], int pLen, int iLen,Tree*_this=this){
			if (!pLen)return 0;
			int root = this->newNode(preorder[0]);
			stack<int> s;
			s.push(root);

			for (int i = 1,irIdx=0; i < pLen; i++) {
				DataType val = preorder[i];
				int n = s.top();
				//一直向左走，没走到最左下角则一直连接当前节点为上一个节点的左子节点，并将当前节点入栈。
				if (t[n].data != inorder[irIdx]) {
					t[n].l =_this-> newNode(val);
					s.push(t[n].l);
				}
				//走到了最左边，开始找右子节点
				else {
					//注意在走到第一个右子节点之前，前序遍历的路径和中序遍历的路径是相反的，所以我们执行弹栈，直到栈顶元素和中序遍历的元素不同，这时得到的就是右子节点。
					while (!s.empty() && t[s.top()].data == inorder[irIdx]) {
						n = s.top(); s.pop();
						++irIdx;
					}
					t[n].r = _this->newNode(val);
					s.push(t[n].r);
				}
			}
			return root;
		}
		
};
	/**
	 * 左右子树高度相差1
	* https://www.bilibili.com/video/BV1rt411j7Ff?spm_id_from=333.337.search-card.all.click&vd_source=c4244c3dee6d867478a203d8882925f0
	*/
	namespace AVL {
		template <typename DataType = int>
		struct AVLNode :TreeNode<DataType> {
			int height;
			int cnt;//压缩存储
			int size;
		};
		template <typename DataType = int>
		struct AVL :Tree<DataType, AVLNode> {
			using TreeType = Tree<DataType, AVLNode>;
			using Tree<DataType, AVLNode>::t;
			using Tree<DataType, AVLNode>::cnt;
			using Tree<DataType, AVLNode>::root;
			using Tree<DataType, AVLNode>::c;
			stack<int> recycle;
			AVL(int c) :TreeType(){
				this->c = c;
				t = new AVLNode<DataType>[c]();
				t[0].data = -1;
			}
			~AVL() { delete[]t; }
			void newNode(int& cur, int data) {
				if (recycle.empty())
					cur = ++cnt;
				else {
					cur = recycle.top(); recycle.pop();
				}
				t[cur].data = data;
				t[cur].size = t[cur].height = t[cur].cnt = 1;
				t[cur].l = t[cur].r = 0;
			}
			int factor(int cur) {
				return t[t[cur].l].height - t[t[cur].r].height;
			}
			void update(int cur) {
				t[cur].size = t[t[cur].l].size + t[t[cur].r].size + t[cur].cnt;
				t[cur].height = max(t[t[cur].l].height, t[t[cur].r].height) + 1;
			}
			void lRotate(int& cur) {
				TreeType::lRotate(cur);
				update(t[cur].l);
				update(cur);
			}
			void rRotate(int& cur) {
				TreeType::rRotate(cur);
				update(t[cur].r);
				update(cur);
			}
			void LL(int& cur) {
				rRotate(cur);
			}
			void LR(int& cur) {
				lRotate(t[cur].l);
				rRotate(cur);
			}
			void RL(int& cur) {
				rRotate(t[cur].r);
				lRotate(cur);
			}
			void RR(int& cur) {
				lRotate(cur);
			}
			/**
			 * update操作写在左旋右旋中需要四次，写在balance中需要三次，但是由于一般balance的节点要多于旋转的节点，所以开销不如直接写在旋转中划算
			*/
			void balance(int& cur) {
				if (!cur)return;
				int f = factor(cur);
				//左子树高
				if (f > 1) {
					if (factor(t[cur].l) >= 0)LL(cur);
					else LR(cur);
				}
				//右子树高
				else if (f < -1) {
					if (factor(t[cur].r) <= 0)RR(cur);
					else RL(cur);
				}
				else update(cur);
			}

			void insert(int& cur, DataType& data) {
				if (!cur) {
					newNode(cur, data);
					return;
				}
				else if (data == t[cur].data)t[cur].cnt++;
				else if (data < t[cur].data)insert(t[cur].l, data);
				else insert(t[cur].r, data);
				balance(cur);//自底向上balance，平衡插入节点的父节点
			}
			/**
			 * 删除当前节点的后继，删除操作的辅助函数，该函数只需要处理节点有两个子节点的情况
			 * 传入参数：当前节点的右节点
			 * 返回：后继节点的下标
			*/
			//lambda表达式的引用捕获可能失灵

			int delSuccessor(int& cur) {
				auto dfs = [&](auto&& self, int& cur)->int {
					//找到了后继
					int ans;
					if (!t[cur].l) {
						ans = cur;
						cur = t[cur].r;//删除后继
					}
					else {
						ans = self(self, t[cur].l);
						balance(cur);
					}
					return ans;
				};
				return dfs(dfs, t[cur].r);
			}

			/**
			 * 需要解决删除留下的空洞 两种解决办法：
			 * 1. 当删除达到指定次数时进行依次碎片整理
			 * 2. 维护一个索引表
			 *
			 * 采用引用就不需要parent
			*/
			int del(int& cur,const DataType& data) {
				if (cur == 0)return 0;
				int p = cur;
				if (t[cur].data == data) {
					if (t[cur].cnt > 1)t[cur].cnt--;
					else {
						int l = t[cur].l, r = t[cur].r;
						//如果只有单个节点,连接父节点和自己的子节点
						if (!t[cur].l || !t[cur].r)
							cur = l + r;
						//如果有两个节点
						else {
							p = delSuccessor(cur);
							t[cur].data = t[p].data;
							t[cur].cnt = t[p].cnt;
						}
						recycle.push(p);
					}
				}
				else if (data < t[cur].data) p = del(t[cur].l, data);
				else p = del(t[cur].r, data);
				balance(cur);
				return p;
			}
			/**
			 * 求大小为x的数的排名
			 * https://www.baeldung.com/cs/node-rank-bst
			*/
			int rank(const DataType& data) {
				int cur = root, rank = 1;
				while (cur) {
					//等于时是+1而不是+size 排名是严格小于x的元素总数加一
					if (t[cur].data >= data)cur = t[cur].l;
					else {
						rank += t[t[cur].l].size + t[cur].cnt;
						cur = t[cur].r;
					}
				}
				return rank;
			}
			/**
			 * 求排名为x的数
			*/
			int findByRank(int rank) {
				int cur = root;
				while (cur) {
					int l = t[cur].l;
					if (t[l].size >= rank)cur = l;
					else {
						rank -= t[l].size + t[cur].cnt;
						if (rank <= 0)return t[cur].data;
						cur = t[cur].r;
					}
				}
			}
			int precursor(const DataType& data) {
				int ans = 0, n = root;
				while (n) {
					if (t[n].data < data) {
						ans = n;
						n = t[n].r;
					}
					else n = t[n].l;
				}
				return ans;
			}
			int successor(const DataType& data) {
				int ans = 0, n = root;
				while (n) {
					if (t[n].data > data) {
						ans = n;
						n = t[n].l;
					}
					else n = t[n].r;
				}
				return ans;
			}
		};
	}
	/**
	* 字典树 用于字符串集中检索单个字符串（时间加速）、词频统计（空间压缩）。
	* 
	*/
	namespace Trie {
		struct TrieNode {
			unordered_map<char, int>cnt;
			unordered_map<char, int>next;
			inline int& operator[](int i) { return next[i]; }
		};

		/**
		*  1.查询ti是多少个sj的前缀。
		*/
		struct Trie {
			int _c, _cnt = 1;
			TrieNode* _t;
			stack<int>recycle;
			Trie(int _c):_c(_c) {
				_t = new TrieNode[_c]();
			}
			~Trie() { delete[]_t; }
			constexpr int root() { return 1; }
			inline int newNode() {
				int p;
				if (recycle.empty())
					p = ++_cnt;
				else {
					p = recycle.top(); recycle.pop();
				}
				_t[p] = { unordered_map<char,int>(),unordered_map<char, int>()};
				return p;
			}
			//查询当前字符串是多少个模式串的前缀
			int countWordsStartWith(const char s[]) {
				int cur = root(), i = 0, child;
				while (s[i+1]) {
					child = _t[cur][s[i]];
					if (!child)return 0;
					cur = child;
					i++;
				}
				return _t[cur].cnt[s[i]];
			}
			//查询当前字符串有多少个在模式串中
			int countWordsEqualTo(const char s[]) {
				int cur = root(), i = 0, child;
				while (s[i + 1]) {
					child = _t[cur][s[i]];
					if (!child)return 0;
					cur = child;
					i++;
				}
				int cnt = _t[cur].cnt[s[i]];
				child = _t[cur][s[i]];
				if(child!=0)for (auto& [key, value] : _t[child].cnt)cnt -= value;

				return cnt;
			}
			void insert(const char s[]) {
				for (int cur = root(), i = 0; s[i]; i++) {
					_t[cur].cnt[s[i]]++;
					int& child = _t[cur][s[i]];
					if (!child)child = newNode();//不支持删除的数据结构 不用写newNode接口
					cur = child;
				}
			}
			void erase(const char s[]) {
				for (int i = 0, cur = root(); s[i]; i++) {
					int& child = _t[cur][s[i]];
					if (--_t[cur].cnt[s[i]] == 0) {
						recycle.push(child);
						child = 0;
					}
					cur = child;
				}
					
			}
			inline void clear() {
				_cnt = 1;
			}
		};
	}

	/**
	* 线段树 用于logn时间内完成区间操作 （前缀和、差分是O(n))
	*/
	namespace SegmentTree {
		template<typename T=int>
		struct STNode {
			T sum, mark;
		};
		template<typename T=int>
		struct SegmentTree {
			using Node = STNode<T>;
			T* _a;
			int _len;
			Node* _t;
			constexpr int root() { return 0; }
			inline int _ls(int p) { return (p << 1) +1; }
			inline int _rs(int p) { return (p << 1) + 2; }
			SegmentTree(T*a,int len):_a(a),_len(len) {
				_t = new Node[_len << 2]();
				build();
			}
			void build(int l, int r, int p ) {
				if (l == r)_t[p].sum = _a[l];
				else {
					int mid = (l + r) >> 1, ls = _ls(p), rs = _rs(p);
					build(l, mid, ls);
					build(mid + 1, r, rs);
					//回溯时更新根节点
					_t[p].sum = _t[ls].sum + _t[rs].sum;
				}
			}
			inline void build() {
				return build(0, _len-1, root());
			}
			void add(int l, int r, T delta, int curl , int curr ,int p) {
				if (curl > r || curr < l)return;//区间无交集
				//全包含
				else if (l <= curl && curr <= r) {
					_t[p].sum += (curr - curl + 1) * delta;
					//非叶子
					if (curr != curl)_t[p].mark += delta;
				}
				//部分包含
				else {
					int mid = (curl + curr) >> 1, ls = _ls(p), rs = _rs(p);
					downLazyTag(p, curr-curl+1);
					add(l, r, delta, curl, mid, ls);
					add(l, r, delta, mid + 1, curr, rs);
					_t[p].sum = _t[ls].sum + _t[rs].sum;
				}
			}

			inline void add(int l, int r, T delta) {
				add(l, r, delta, 0, _len-1, root());
			}
			T query(int l,int r,int curl,int curr,int p) {
				if (curl > r || curr < l)return 0;
				else if (l <= curl && curr <= r)return _t[p].sum;
				else {
					int mid = (curl + curr) >> 1;
					downLazyTag(p, curr - curl + 1);
					return query(l, r, curl, mid, _ls(p)) + query(l, r, mid + 1, curr, _rs(p));
				}
			}
			inline T query(int l, int r) {
				return query(l, r, 0, _len-1, root());
			}
			void downLazyTag(int p,int len) {
				int ls = _ls(p), rs = _rs(p);
				_t[ls].mark += _t[p].mark;
				_t[rs].mark += _t[p].mark;
				_t[ls].sum += _t[p].mark * (len-len/2);
				_t[rs].sum += _t[p].mark * (len/2);//右边短一点
				_t[p].mark = 0;
			}
		};
	}
	namespace Heap {
		//默认大根堆
		template<typename DataType=int,template<typename>typename Greater=greater>
		struct Heap {
			DataType* _t;//从1开始
			int _c;
			unsigned int _size = 0;
			DataType defaultValue = DataType();
			Greater<DataType> greater;
			Heap(int c):_c(c) {_t = new DataType[c]();}
			Heap(DataType*t,int c):_t(t-1),_c(c),_size(c+1) {}
			~Heap() { delete[] _t; }
			constexpr int _root() { return 1; }
			//if else 很可能不内联，三元运算符更可能内联，导致显著的速度差异
			inline int _ls(int i) { return (i <<1<= _size)? i << 1: 0; }
			inline int _rs(int i) { return((i<<1)+1 <= _size)?(i << 1) + 1: 0; }
			inline int _parent(int i) { return i >> 1; }
			inline DataType& top() { return _size ? _t[1] : defaultValue; }
			inline unsigned int size() { return _size; }
			//从堆底开始向上调整堆
			void siftUp(int i) {
				while (i > 1) {
					int p = _parent(i);
					if (greater(_t[i],_t[p]))
						swap(_t[i], _t[p]);
					else break;
					i=p;
				}
			}
			//从堆顶开始向下调整堆
			void siftDown(int i) {
				while (true) {
					int l = _ls(i), r =_rs(i);
					int large = i;
					if (l&&greater(_t[l], _t[large]))large = l;
					if (r&&greater(_t[r], _t[large]))large = r;
					if (large != i) {
						swap(_t[i], _t[large]);
						i = large;
					}
					else break;
				}
			}
			template<typename U=DataType>
			void insert(U&& data) {
				_t[++_size] = data;
				siftUp(_size);
			}
			void del(int node) {
				swap(_t[_size], _t[node]);
				_size--;
				if (node>1&&greater(_t[node] ,_t[_parent(node)]))siftUp(node);
				else siftDown(node);
			}
			void pop() {
				swap(_t[_size], _t[_root()]);
				_size--;
				siftDown(_root());
			}
			
		};
	}
}