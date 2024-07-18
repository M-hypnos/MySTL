#include "queueTest.h"
#include "testHelper.h"
#include <assert.h>

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

		void test() {
			test1();
			test2();
			test3();
			test4();
			test5();
		}
	}
}