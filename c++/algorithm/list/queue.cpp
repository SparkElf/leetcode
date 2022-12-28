#include <iostream>
#include <vector>
using namespace std;
namespace Queue {
	template <typename T = int>
	struct Queue {
		T* _q;
		int _c;
		int _size = 0;
		int _front = 0, _back = -1;
		T _fail;
		Queue() {}
		Queue(int _c, T&& _fail = T()) :_c(_c), _fail(_fail) {
			_q = new T[_c];
		}
		~Queue() { delete[]_q; }
		T& operator[](int i) { return _q[i]; }
		inline bool empty() { return _size == 0; }
		inline bool full() { return _size == _c; }
		template<typename U = T>
		bool push_front(U&& data) {
			if (full()) return false;
			_front = (_front - 1 + _c) % _c;
			_q[_front] = forward<U>(data);
			_size++;
			return true;
		}
		template<typename U = T>
		bool push_back(U&& data) {
			if (full()) return false;
			_back = (_back + 1) % _c;
			_q[_back] = forward<U>(data);
			_size++;
			return true;

		}
		bool pop_back() {
			if (empty()) return false;
			_size--;
			_back = (_back - 1 + _c) % _c;
			return false;
		}
		bool pop_front() {
			if (empty()) return false;
			_size--;
			_front = (_front + 1) % _c;
			return true;
		}
		T& front() { if (!empty()) return _q[_front]; else return _fail; }
		T& back() { if (!empty())return _q[_back]; else return _fail; }
	};
}

int main() {

	return 0;
}