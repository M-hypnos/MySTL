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

			for (auto i = 0; i != 100; ++i) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			assert(mySTL::testHelper::equal(dq1, dq2));

			for (auto i = 10; i != 200; ++i) {
				dq1.push_front(i);
				dq2.push_front(i);
			}
			assert(mySTL::testHelper::equal(dq1, dq2));
			for (auto i = 0; i != 100; ++i) {
				dq1.pop_back();
				dq2.pop_back();
			}
			assert(mySTL::testHelper::equal(dq1, dq2));
			for (auto i = 10; i != 200; ++i) {
				dq1.pop_front();
				dq2.pop_front();
			}
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

			assert(foo1 != bar);
			auto foo2 = bar;
			assert(foo2 == bar);
		}
		void test7() {
			int arr[] = { 1, 2, 4, 5, 5, 6 };
			stdQueue<int> dq1(arr, arr + 6);
			myQueue<int> dq2(arr, arr + 6);
			dq1.at(1) = 88;
			dq2.at(1) = 88;
			dq1[2] = 33;
			dq2[2] = 33;
			assert(mySTL::testHelper::equal(dq1, dq2));
		}
		void test8() {
			int arr[] = { 1, 2, 3, 4, 5 };
			myQueue<int> dq(arr, arr + 5);

			auto i = 1;
			for (myQueue<int>::iterator it = dq.begin(); it != dq.end(); ++it, ++i) {
				assert(*it == i);
			}

			myQueue<int> dq1(5);  // 5 default-constructed ints
			i = 0;
			myQueue<int>::reverse_iterator rit = dq1.rbegin();
			for (; rit != dq1.rend(); ++rit)
				*rit = ++i;

			i = 5;
			for (myQueue<int>::iterator it = dq1.begin(); it != dq1.end(); ++it, --i) {
				assert(*it == i);
			}

			i = 1;
			for (myQueue<int>::reverse_iterator it = dq1.rbegin(); it != dq1.rend(); ++it, ++i) {
				assert(*it == i);
			}
		}
		void test9() {
			myQueue<int> dq1;
			stdQueue<int> dq2;
			int i = 1;
			while (i < 60) {
				dq1.push_back(i);
				dq2.push_back(i);
				i++;
			}
			i = 1;
			while (i < 20) {
				dq1.push_front(i);
				dq2.push_front(i);
				i++;
			}
			assert(mySTL::testHelper::equal(dq1, dq2));
			dq1.clear();
			dq2.clear();
			assert(mySTL::testHelper::equal(dq1, dq2));
			i = 1;
			while (i < 70) {
				dq1.push_back(i);
				dq2.push_back(i);
				i++;
			}
			i = 1;
			while (i < 40) {
				dq1.push_front(i);
				dq2.push_front(i);
				i++;
			}
			assert(mySTL::testHelper::equal(dq1, dq2));

			dq1.insert(dq1.begin(), 200, 100);
			dq2.insert(dq2.begin(), 200, 100);
			assert(mySTL::testHelper::equal(dq1, dq2));

			dq1.insert(dq1.end(), 400, 100);
			dq2.insert(dq2.end(), 400, 100);
			assert(mySTL::testHelper::equal(dq1, dq2));
		}
		void test10() {
			myQueue<int> dq1;
			stdQueue<int> dq2;
			int num = 2000;
			for (int i = 0; i < num; i++) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			assert(dq1.begin() - dq1.end() == dq2.begin() - dq2.end());
			assert(dq1.end() - dq1.begin() == dq2.end() - dq2.begin());
			for (int i = 0; i < num; i++) {
				assert(*(dq1.begin() + i) == *(dq1.end() - (num - i)));
			}
			auto it1 = dq1.erase(dq1.begin());
			auto it2 = dq2.erase(dq2.begin());
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.erase(dq1.begin(), dq1.begin()+400);
			it2 = dq2.erase(dq2.begin(), dq2.begin()+400);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			for (auto it = dq1.begin(); it != dq1.end();)
			{
				if (*it % 2 == 0)
					it = dq1.erase(it);
				else
					++it;
			}
			for (auto it = dq2.begin(); it != dq2.end();)
			{
				if (*it % 2 == 0)
					it = dq2.erase(it);
				else
					++it;
			}
			assert(mySTL::testHelper::equal(dq1, dq2));
		}
		void test11() {
			myQueue<int> dq1;
			stdQueue<int> dq2;
			for (int i = 0; i < 2000; i++) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			myQueue<int> dq3;
			for (int i = 0; i < 4000; i++) {
				dq3.push_back(i);
			}
			dq3 = dq1;
			assert(mySTL::testHelper::equal(dq3, dq2));
			for (int i = 0; i < 2000; i++) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			dq3 = dq1;
			assert(mySTL::testHelper::equal(dq3, dq2));
		}
		void test12() {
			myQueue<int> dq1(300,100);
			stdQueue<int> dq2(300, 100);
			auto it1 = dq1.insert(dq1.begin() + 50, 700, 20);
			auto it2 = dq2.insert(dq2.begin() + 50, 700, 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.begin() + 50, 30, 20);
			it2 = dq2.insert(dq2.begin() + 50, 30, 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.end() - 50, 700, 20);
			it2 = dq2.insert(dq2.end() - 50, 700, 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.end() - 50, 30, 20);
			it2 = dq2.insert(dq2.end() - 50, 30, 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.begin(), 20);
			it2 = dq2.insert(dq2.begin(), 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.begin() + 50, 20);
			it2 = dq2.insert(dq2.begin() + 50, 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.end(), 20);
			it2 = dq2.insert(dq2.end(), 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));
			it1 = dq1.insert(dq1.end() - 50, 20);
			it2 = dq2.insert(dq2.end() - 50, 20);
			assert(it1 - dq1.begin() == it2 - dq2.begin());
			assert(mySTL::testHelper::equal(dq1, dq2));

			myQueue<int> dq3;
			stdQueue<int> dq4;
			for (int i = 0; i < 1000; i++) {
				dq3.push_back(i);
				dq4.push_back(i);
			}
			it1 = dq3.insert(dq3.begin() + 400, 400, 20);
			it2 = dq4.insert(dq4.begin() + 400, 400, 20);
			assert(it1 - dq3.begin() == it2 - dq4.begin());
			assert(mySTL::testHelper::equal(dq3, dq4));
			it1 = dq3.insert(dq3.end() - 400, 400, 20);
			it2 = dq4.insert(dq4.end() - 400, 400, 20);
			assert(it1 - dq3.begin() == it2 - dq4.begin());
			assert(mySTL::testHelper::equal(dq3, dq4));

			dq1.clear();
			dq2.clear();
			for (int i = 0; i < 1000; i++) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			dq3.clear();
			dq4.clear();
			for (int i = 1000; i < 1700; i++) {
				dq3.push_back(i);
				dq4.push_back(i);
			}
			it1 = dq3.insert(dq3.begin() + 200, dq1.begin() + 400, dq1.begin() + 500);
			it2 = dq4.insert(dq4.begin() + 200, dq2.begin() + 400, dq2.begin() + 500);
			assert(it1 - dq3.begin() == it2 - dq4.begin());
			assert(mySTL::testHelper::equal(dq3, dq4));
			it1 = dq3.insert(dq3.begin() + 200, dq1.begin() + 400, dq1.begin() + 800);
			it2 = dq4.insert(dq4.begin() + 200, dq2.begin() + 400, dq2.begin() + 800);
			assert(it1 - dq3.begin() == it2 - dq4.begin());
			assert(mySTL::testHelper::equal(dq3, dq4));
			it1 = dq3.insert(dq3.end() - 200, dq1.begin() + 400, dq1.begin() + 500);
			it2 = dq4.insert(dq4.end() - 200, dq2.begin() + 400, dq2.begin() + 500);
			assert(it1 - dq3.begin() == it2 - dq4.begin());
			assert(mySTL::testHelper::equal(dq3, dq4));
			it1 = dq3.insert(dq3.end() - 200, dq1.begin() + 400, dq1.begin() + 800);
			it2 = dq4.insert(dq4.end() - 200, dq2.begin() + 400, dq2.begin() + 800);
			assert(it1 - dq3.begin() == it2 - dq4.begin());
			assert(mySTL::testHelper::equal(dq3, dq4));
		}
		void test13() {
			myQueue<int> dq1;
			stdQueue<int> dq2;
			for (int i = 1; i < 4; i++) {
				dq1.push_back(i);
				dq2.push_back(i);
			}
			dq1.resize(5);
			dq2.resize(5);
			assert(mySTL::testHelper::equal(dq1, dq2));
			dq1.resize(2);
			dq2.resize(2);
			assert(mySTL::testHelper::equal(dq1, dq2));
			dq1.resize(6,4);
			dq2.resize(6, 4);
			assert(mySTL::testHelper::equal(dq1, dq2));
		}
		class TestItem
		{
		public:
			TestItem()
			{
				++count;
			}
			TestItem(const TestItem& other)
			{
				++count;
			}

			virtual ~TestItem()
			{
				--count;
			}

			static int getCount()
			{
				return count;
			}
		private:
			static int count;
		};
		int TestItem::count = 0;
		void test14() {
			assert(TestItem::getCount() == 0);
			{
				myQueue<TestItem> dq1(10);
				dq1.push_back(TestItem());
				dq1.push_back(TestItem());
				dq1.push_back(TestItem());
				dq1.push_front(TestItem());
				dq1.push_front(TestItem());
				dq1.push_front(TestItem());

				dq1.insert(dq1.begin(), dq1.begin(), dq1.begin() + 1);

				myQueue<TestItem> dq2(10);
				dq2.push_back(TestItem());
				dq2.push_back(TestItem());
				dq2.push_back(TestItem());
				dq2.push_front(TestItem());
				dq2.push_front(TestItem());
				dq2.push_front(TestItem());
				dq2.insert(dq2.begin(), dq2.begin(), dq2.begin() + 1);

			}
			assert(TestItem::getCount() == 0);
		}

		void test() {
			test1();
			test2();
			test3();
			test4();
			test5();
			test6();
			test7();
			test8();
			test9();
			test10();
			test11();
			test12();
			test13();
			test14();
		}
	}
}