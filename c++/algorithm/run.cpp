#include<vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <list>
#include <stack>
#include <queue>
using namespace std;

int bf(string& s, string& pattern) {
	int i = 0, j = 0;
	while (i < s.length() && j < pattern.length()) {
		if (s[i] == pattern[j]) { ++i; ++j; }
		else { i = i - j + 1; j = 0; }
	}
	//0 1 2 3 0 1 2 4-3=1
	if (j == pattern.length())return i - j;
	return -1;
}
int main() {
	
}