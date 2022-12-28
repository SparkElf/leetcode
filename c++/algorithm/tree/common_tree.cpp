#include <vector>
using namespace std;
namespace CommonTree {
	/**
	* 无向树的直径  O(n)  https://leetcode.cn/problems/tree-diameter/
	*/
	int diameter(vector<vector<int>>edges) {
		vector<vector<int>>g;
		g.resize(edges.size() + 1);//树的顶点树是边数+1
		for (auto& e : edges) {
			int a = e[0], b = e[1];
			g[a].push_back(b);
			g[b].push_back(a);//可去掉，在题目中
		}
		int ans = 0;
		//返回最大高度
		auto dfs = [&](auto&&self,int cur,int parent)->int {
			int d1=0, d2=0;
			for (auto child:g[cur]) {
				if (child == parent)continue;
				int d = self(self, child, cur);
				if (d > d1) {d2 = d1;d1 = d;}
				else if (d > d2)d2 = d;
			}
			ans = max(ans, d1 + d2 + 1);
			return d1+1;
		};
		dfs(dfs, 0, 0);
		return ans-1;
	}

}