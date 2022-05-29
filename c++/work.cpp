#include <bits/stdc++.h>
using namespace std;
#define range(i,start,end) for(int i=start;i<end;i++)
#define DEBUG

auto g = unordered_map<int, unordered_map<int, int>>();

struct MainObjectNode {
    int id, value;
    bool operator <(const MainObjectNode& obj)const {//降序排列要反着来
        return value > obj.value || (value == obj.value && id < obj.id);
    }
};

auto main_object = unordered_map<int, set<MainObjectNode>>();

struct CounterNode {
    int u, v, x;
};
//按截止日期后一天索引的申请信息的统计数组 截止日期后一天=申请日期+y
auto counter = unordered_map<int, vector<CounterNode>>();
bool hasdui(int u) {
    return !main_object[u].empty()&&main_object[main_object[u].begin()->id].begin()->id == u;
}
bool isdui(int u, int v) {
    return !main_object[u].empty() && !main_object[v].empty() && main_object[u].begin()->id == v && main_object[v].begin()->id == u;
}
int main() {
#ifdef DEBUG
    ifstream cin("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\input.txt");
    ofstream cout("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\output.txt");
#endif
    /*
        主要解法为动态维护，而不是每次询问都遍历查询
        编号从1开始，天数从1开始，申请的额度大于0
    */
    int n, m;
    cin >> n >> m;
    int k, l, o, p, q;
    int u, v, x, y;
    int gudao = n, dui = 0;
    range(i, 1, m + 1) {
        //清理过期申请
        for (auto item : counter[i]) {
            u = item.u, v = item.v, x = item.x;
            bool wasdui = isdui(u,v);
            main_object[u].erase({ v,g[u][v] });
            main_object[v].erase({ u,g[v][u] });
            g[u][v] -= x;
            g[v][u] -= x;
            int value = g[u][v];
            if (value != 0) {
                main_object[u].insert({ v,value });
                main_object[v].insert({ u,value });
            }
            else {
                if (main_object[u].empty())gudao++;
                if (main_object[v].empty())gudao++;
            }
            if (!isdui(u,v)&&wasdui) {
                dui--;
                if (hasdui(u))dui++;
                if (hasdui(v))dui++;
            }

        }

        cin >> k;
        range(j, 0, k) {
            cin >> u >> v >> x >> y;
            counter[i + y].push_back({ u,v,x });
            if (main_object[u].empty())gudao--;
            if (main_object[v].empty())gudao--;
            bool wasdui = isdui(u,v);
            bool haddui_u = hasdui(u), haddui_v = hasdui(v);
            main_object[u].erase({ v,g[u][v] });
            main_object[v].erase({ u,g[v][u] });
            g[u][v] += x;
            g[v][u] += x;
            int value = g[u][v];
            main_object[u].insert({ v,value });
            main_object[v].insert({ u,value });
            if (isdui(u,v)&& !wasdui) {
                if (haddui_u)dui--;
                if (haddui_v)dui--;
                dui++;
            }
        }
        cin >> l;
        range(j, 0, l) {
            cin >> o;
            cout << main_object[o].begin()->id<< endl;
        }
        cin >> p >> q;
        if (p) cout << gudao << endl;
        if (q) cout << dui << endl;
    }
    return 0;
}