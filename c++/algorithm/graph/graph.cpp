#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
using namespace std;

namespace Graph {
    constexpr int INF = 0x3f3f3f;
    template<class T>
    struct GMartrix {
        T** martrix;
        uint32_t vertexNum;
        GMartrix(uint32_t size) {
            this->vertexNum = size;
            martrix = new T * [size];
            for (int i = 0; i < size; i++)martrix[i] = new T[size]();
        }
        ~GMartrix() {
            for (int i = 0; i < vertexNum; i++)
                delete[]martrix[i];
            delete[]martrix;
        }
        T*& operator [](int i) {
            return martrix[i];
        }
        void fill_data(T data) {
            for (int i = 0; i < vertexNum; i++)
                for (int j = 0; j < vertexNum; j++)
                    martrix[i][j] = data;
        }
    };
    /**
     * 前提：g中不可达的边权值为+INF
     */
    bool flyod(GMartrix<int> g) {
        for (int k = 0; k < g.vertexNum; k++)
            for (int i = 0; i < g.vertexNum; i++)
                for (int j = 0; j < g.vertexNum; j++)
                    g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
        for (int i = 0; i < g.vertexNum; i++)
            if (g[i][i] < 0)return false;//有环
        return false;
    }
    struct DisjointSet {
        vector<int>_parent;
        vector<int>_rank;
        DisjointSet(int n) :_parent(n),_rank(n) {
            iota(_parent.begin(), _parent.end(), 0);
        }
        int find(int x){
            int ans=x;
            while (ans != _parent[ans])
                ans = _parent[ans];
            //路径压缩
            int nxt;
            while (x != ans) {
                nxt = _parent[x];
                _parent[x] = ans;
                x = nxt;
            }
            return ans;
        }
        bool merge(int x, int y) {
            x = find(x);y = find(y);
            if (x == y)return false;
            //按秩合并矮树合并到高树 注意_rank并不实际反应树高只是节点数多的集合高度可能更大
            if (_rank[x] < _rank[y])_parent[x] = y;
            else {
                _parent[y] = x;
                if (_rank[x] == _rank[y])_rank[x]++;
            }
            return true;
        }

    };
    //https://www.notion.so/ceec35551e564386ae12284ce643ec2c#e0c7ed711f5041ac9192b0faf94d2265
    //m n分别为左、右元素数量
    int hungarian(unordered_map<int, vector<int>>g) {
        int cnt = 0;
        unordered_set<int>vis;
        unordered_map<int, int>match;
        auto enLarge = [&](auto&& self, int x)->bool {
            auto& children = g[x];
            for (auto y : g[x])
                if (vis.find(y) == vis.end()) {
                    vis.insert(y);
                    if (match.find(y) == match.end() || self(self, match[y])) {
                        match[y] = x;
                        return true;
                    }
                }
            return false;
        };
        for (auto& p : g) {
            if (enLarge(enLarge, p.first))cnt++;
            vis.clear();
        }
        return cnt;
    }