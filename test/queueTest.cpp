#include "queueTest.h"
#include "testHelper.h"
#include <assert.h>
#include <deque>
#include "../functors.h"
#include "../container/vector.h"
#include "../container/deque.h"
#include <random>

namespace mySTL {
	namespace queueTest {
		void test1() {
			stdQueue<int> q1;
			myQueue<int> q2;

			for (auto i = 0; i != 10; ++i) {
				q1.push(i);
				q2.push(i);
			}
			for (auto i = 0; i != 10; ++i) {
				assert(q1.front() == q2.front());
				q1.pop();
				q2.pop();
			}
		}
		void test2() {
			myQueue<int> q1;
			for (auto i = 0; i != 10; ++i)
				q1.push(i);
			auto q2(q1);
			assert(q1 == q2);
			assert(!(q1 != q2));
		}
		void test3() {
			myQueue<int> q;
			assert(q.empty());
			assert(q.size() == 0);

			q.push(10);
			q.push(11);
			assert(!q.empty());
			assert(q.size() == 2);
		}
		void test4() {
			myQueue<std::string> q;
			q.push("front");
			q.push("back");

			assert(q.front() == "front");
			assert(q.back() == "back");
		}
		void test5() {
			myQueue<int> q1, q2;

			q1.push(1); q1.push(2); q1.push(3);
			q2.push(1); q2.push(2);

			assert(q1.size() == 3 && q2.size() == 2);
			q1.swap(q2);
			assert(q1.size() == 2 && q2.size() == 3);
		}
		void test6() {
			std::vector<int> vec1{ 3, 1, 4, 1, 5 };
			std::priority_queue<int> pq1(std::less<int>(), vec1);
			mySTL::vector<int> vec2(vec1.begin(), vec1.end());
			mySTL::priority_queue<int> pq2(mySTL::less<int>(), vec2);
			assert(pq1.top() == pq2.top());

			std::priority_queue<int> pq11;
			mySTL::priority_queue<int> pq22;
			pq11.swap(pq1);
			pq22.swap(pq2);
			assert(pq1.top() == pq2.top());
			assert(pq11.top() == pq22.top());

			std::priority_queue<int, std::vector<int>, std::greater<int>> pq3(std::greater<int>(), vec1);
			mySTL::priority_queue<int, mySTL::vector<int>, mySTL::greater<int>> pq4(mySTL::greater<int>(), vec2);
			assert(pq3.top() == pq4.top());

			pq3.push(1);
			pq4.push(1);
			assert(pq3.top() == pq4.top());
			pq3.push(5);
			pq4.push(5);
			assert(pq3.top() == pq4.top());
			pq3.push(7);
			pq4.push(7);
			assert(pq3.top() == pq4.top());
			pq3.push(3);
			pq4.push(3);
			assert(pq3.top() == pq4.top());

			while (!pq3.empty() && !pq4.empty()) {
				assert(pq3.top() == pq4.top());
				pq3.pop();
				pq4.pop();
			}

			assert(pq3.size() == pq4.size());
		}

		void test() {
			test1();
			test2();
			test3();
			test4();
			test5();
			test6();
		}
	}
}