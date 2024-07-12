#include "dequeTest.h"
#include "testHelper.h"
#include <assert.h>
#include <random>
namespace mySTL {
	namespace dequeTest {
		void test1() {
			stdQueue<int> dq1(10, 0);
			myQueue<int> dq2(10, 0);
			assert(mySTL::testHelper::equal(dq1, dq2));
			
			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdQueue<int> dq3(std::begin(arr), std::end(arr));
			myQueue<int> dq4(std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal(dq3, dq4));

			auto dq5(dq1);
			auto dq6(dq2);
			assert(mySTL::testHelper::equal(dq5, dq6));

			auto dq7 = dq3;
			auto dq8 = dq4;
			assert(mySTL::testHelper::equal(dq7, dq8));

			auto dq9 = std::move(dq7);
			auto dq10 = std::move(dq8);
			assert(mySTL::testHelper::equal(dq9, dq10));
		}
		void test2() {
			myQueue<int> dq1;
			assert(dq1.empty());
			assert(dq1.size() == 0);

			myQueue<int> dq2(10, 0);
			assert(!dq2.empty());
			assert(dq2.size() == 10);
		}
		void test3() {
			stdQueue<std::string> dq1(10, "10");
			myQueue<std::string> dq2(10, "10");

			dq1[0] = "0"; dq1[9] = "9";
			dq2[0] = "0"; dq2[9] = "9";

			assert(dq1.front() == dq2.front());
			assert(dq1.back() == dq2.back());
		}
		void test4() {
			stdQueue<int> dq1;
			myQueue<int> dq2;

			for (auto i = 0; i != 10; ++i) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			assert(mySTL::testHelper::equal(dq1, dq2));

			for (auto i = 10; i != 20; ++i) {
				dq1.push_front(i);
				dq2.push_front(i);
			}
			assert(mySTL::testHelper::equal(dq1, dq2));

			for (auto i = 0; i != 5; ++i) {
				dq1.pop_back();
				dq2.pop_back();
			}
			assert(mySTL::testHelper::equal(dq1, dq2));

			for (auto i = 0; i != 5; ++i) {
				dq1.pop_front();
				dq2.pop_front();
			}
			assert(mySTL::testHelper::equal(dq1, dq2));
		}
		void test5() {
			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			myQueue<int> foo(arr, arr + 3), bar(arr + 3, arr + 10);

			assert(foo.size() == 3 && bar.size() == 7);
			foo.swap(bar);
			assert(foo.size() == 7 && bar.size() == 3);
			mySTL::swap(foo, bar);
			assert(foo.size() == 3 && bar.size() == 7);
		}
		void test6() {
			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			myQueue<int> foo1(arr, arr + 3), bar(arr + 3, arr + 10);

			/*assert(foo1 != bar);
			auto foo2 = bar;
			assert(foo2 == bar);*/
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