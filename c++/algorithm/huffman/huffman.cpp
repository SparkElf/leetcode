/**
 * An Experiment by c++ 20
 */
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <cmath>
#include <fstream>
#include <filesystem>
using namespace std;

namespace Huffman {
#define range(i,start,_end) for(int i=start;i<_end;i++)
#define ch_loop(ch,in) for(char ch=in.get();ch!=EOF;ch=in.get())
    using size_t = unsigned int;

    struct Node {
        size_t freq = 0;
        char ch = -1;
        int lc = -1, rc = -1, parent = -1;

    };
    struct CompressInfo {
        size_t originalSize = 0, compressedSize = 0;
    };
    void build_tree(vector<Node>& nodes) {
        //构建哈夫曼树
        auto cmp = [](Node& a, Node& b) {return a.freq > b.freq;};//https://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator
        auto q = priority_queue<Node, vector<Node>, decltype(cmp)>(nodes.begin(), nodes._end());//https://iq.opengenus.org/initialize-priority-queue-in-cpp/

        //边界情况
        if (nodes.size() == 1) {
            nodes[0] = Node{ .lc = 1 };
            nodes[1] = q.top();
            nodes[1].parent = 0;
            return;
        }

        nodes = vector<Node>(2 * nodes.size() - 1);//https://stackoverflow.com/questions/9293674/can-we-reassign-the-reference-in-c

        for (int i = nodes.size() - 1;i > 0;i -= 2) {
            nodes[i] = q.top();q.pop();
            nodes[i - 1] = q.top();q.pop();
            q.push(Node{ .lc = i - 1,.rc = i });

            int l = nodes[i].lc, r = nodes[i].rc;
            if (l != -1)nodes[l].parent = i;
            if (r != -1)nodes[r].parent = i;

            l = nodes[i - 1].lc, r = nodes[i - 1].rc;
            if (l != -1)nodes[l].parent = i - 1;
            if (r != -1)nodes[r].parent = i - 1;
        }
        nodes[0] = q.top();
    }
    struct Compressor {
        size_t capacity;
        vector<Node> nodes;
        unordered_map<char, pair<int, string>>codingTable;//频率和码
        istream& src;//stream不可复制，只能使用指针或者引用

        bool encoded = false;
        CompressInfo info;

        //默认只解码标准ascii字符 支持字符流和文件流
        Compressor(istream &src, size_t capacity = 256) :capacity(capacity), nodes(vector<Node>(capacity)), src(src) {}
        auto encode()->Compressor& {
            //边界情况
            if (src.peek() == EOF)
                throw "empty string not allow";
            //使用256哈希表统计频率
            range(i, 0, capacity)
                nodes[i].ch = i;

            ch_loop(ch, src)
                nodes[ch].freq++;

            erase_if(nodes, [](const Node& n) {return n.freq == 0;});

            build_tree(nodes);

            function<void(int, string)> create_coding_table = [&](int n, string code) {
                if (nodes[n].ch != -1)codingTable[nodes[n].ch] = make_pair(nodes[n].freq, move(code));
                else {
                    if (nodes[n].lc != -1)create_coding_table(nodes[n].lc, code + "0");
                    if (nodes[n].rc != -1)create_coding_table(nodes[n].rc, code + "1");
                }
            };
            create_coding_table(0, "");
            //性能信息
             for (auto item : codingTable) {
                 info.compressedSize += item.second.first * item.second.second.size();
                 info.originalSize += item.second.first * 8;
             }
            encoded = true;
            return *this;
        };
        auto toFile(filesystem::path&& p)->Compressor& {
            src.clear();//删除EOF标志位
            src.seekg(0);
            ofstream file(p);
            file << codingTable.size() << endl;
            for (auto iter = codingTable.begin();iter != codingTable._end();iter++) { file << iter->first << " " << iter->second.first << endl; }
            ch_loop(ch,src)
                file << codingTable[ch].second;
            return *this;
        }

    };
    struct Decompressor {
        vector<Node> nodes;
        istream& src;
        Decompressor(istream&src) :nodes(vector<Node>()), src(src) {}
        auto decode(ostream& dest) ->void {
            int size;
            src >> size;
            src.get();//换行符
            range(i, 0, size) {
                char ch = src.get();
                src.get();
                size_t freq;
                src >> freq;
                src.get();
                nodes.push_back({ freq,ch });
            }
            build_tree(nodes);
            int cur = 0;
            for (char ch = src.get();ch == '1' || ch == '0';ch = src.get()) {
                if (ch == '0')cur = nodes[cur].lc;
                else if (ch == '1')cur = nodes[cur].rc;
                if (nodes[cur].ch != -1) {
                    dest << nodes[cur].ch;
                    cur = 0;
                }
            }
        }
    };

    auto print(vector<Node>& tree, filesystem::path&& filepath) ->void {//https://blog.csdn.net/Ice__Clean/article/details/121078026?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-121078026-blog-112726668.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-121078026-blog-112726668.pc_relevant_default&utm_relevant_index=2
        ofstream cout(filepath);
        auto print_node = [&](int n) {
            if (tree[n].ch == -1)cout << "none" << endl;
            else if (isdigit(tree[n].ch) || isalpha(tree[n].ch)) cout << tree[n].ch << endl;
            else cout << "special-" << (int)tree[n].ch << endl;
        };
        function<void(string&, int, bool)>dfs = [&](const string& prefix, int n, bool isLeft)->void {
            if (n != -1) {
                cout << prefix;
                cout << (isLeft ? "├──":"└──" );

                // print the value of the node
                print_node(n);

                // enter the next tree level - left and right branch
                string prefix1 = prefix + (isLeft ? "│   " : "    ");
                string prefix2 = prefix1;
                dfs(prefix1, tree[n].lc, true);
                dfs(prefix2, tree[n].rc, false);
            }
        };
        auto prefix = string("");
        dfs(prefix, 0, false);
    }
    auto print(unordered_map<char, pair<int, string>>& codingTable, filesystem::path&& filepath)->void {
        ofstream file(filepath);
        for (auto iter = codingTable.begin();iter != codingTable._end();iter++) { file << iter->first << " " << iter->second.second << endl; }
    }
    auto print(CompressInfo& info, filesystem::path&& filepath)->void {
        ofstream file(filepath);
        file << "原始大小:"<<info.originalSize<<endl;

        file << "压缩大小:"<<info.compressedSize<<endl;

        file << "压缩率:"<<(int)(info.compressedSize/(double)info.originalSize*  100 ) <<"%"<<endl;
    }

#undef range
#undef ch_loop
}

