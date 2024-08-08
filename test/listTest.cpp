#include "listTest.h"
#include "testHelper.h"
#include <assert.h>
#include <random>
namespace mySTL {
	namespace listTest {
		void test1() {
			stdList<int> l1(10, 0);
			myList<int> l2(10, 0);
			assert(mySTL::testHelper::equal(l1, l2));
			
			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdList<int> l3(std::begin(arr), std::end(arr));
			myList<int> l4(std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal(l3, l4));

			auto l5(l1);
			auto l6(l2);
			assert(mySTL::testHelper::equal(l5, l6));

			stdList<int> l7(15, 10);
			myList<int> l8(15, 10);
			l7 = l1;
			l8 = l2;
			assert(mySTL::testHelper::equal(l7, l8));
			stdList<int> l9(5, 10);
			myList<int> l10(5, 10);
			l9 = l1;
			l10 = l2;
			assert(mySTL::testHelper::equal(l9, l10));
		}

		void test2() {
			myList<std::string> l1;
			assert(l1.empty());
			assert(l1.size() == 0);

			std::string arr[] = { "1", "2", "3" };
			myList<std::string> l2(std::begin(arr), std::end(arr));
			assert(!l2.empty());
			assert(l2.size() == 3);
		}
		void test3() {
			std::string arr[] = { "1", "2", "3" };
			myList<std::string> l(std::begin(arr), std::end(arr));
			assert(l.back() == arr[2]);
			assert(l.front() == arr[0]);

			l.front() = "front";
			l.back() = "back";
			assert(l.back() == "back");
			assert(l.front() == "front");
		}
		void test4() {
			stdList<int> l1;
			myList<int> l2;
			for (auto i = 0; i != 10; ++i) {
				l1.push_front(i);
				l2.push_front(i);
			}
			assert(mySTL::testHelper::equal(l1, l2));
			for (auto i = 0; i != 10; ++i) {
				l1.push_back(i);
				l2.push_back(i);
			}
			assert(mySTL::testHelper::equal(l1, l2));
			for (auto i = 0; i != 5; ++i) {
				l1.pop_back();
				l2.pop_back();
			}
			assert(mySTL::testHelper::equal(l1, l2));
			for (auto i = 0; i != 5; ++i) {
				l1.pop_front();
				l2.pop_front();
			}
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test5() {
			stdList<int> l1;
			myList<int> l2;

			for (auto i = 0; i != 10; ++i) {
				l1.push_back(i);
				l2.push_back(i);
			}

			int i = 0;
			for (myList<int>::const_iterator it = l2.cbegin(); it != l2.cend(); ++it, ++i) {
				assert(*it == i);
			}

			auto rit1 = l1.rbegin();
			auto rit2 = l2.rbegin();
			for (; rit1 != l1.rend() && rit2 != l2.rend(); ++rit1, ++rit2) {
				assert(*rit1 == *rit2);
			}
			assert(rit1 == l1.rend() && rit2 == l2.rend());
		}
		void test6() {
			stdList<int> l1;
			myList<int> l2;

			l1.insert(l1.end(), 10, -1);
			l2.insert(l2.end(), 10, -1);
			assert(mySTL::testHelper::equal(l1, l2));

			auto it1 = l1.begin();
			auto it2 = l2.begin();
			for (auto i = l1.size() / 2 + 1; i != 0; --i) {
				++it1;
				++it2;
			}
			l1.insert(it1, 1);
			l2.insert(it2, 1);
			assert(mySTL::testHelper::equal(l1, l2));

			int arr[] = { 1, 2, 3, 4, 5 };
			it1 = l1.end();
			it2 = l2.end();
			l1.insert(it1, std::begin(arr), std::end(arr));
			l2.insert(it2, std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test7() {
			stdList<int> l1;
			myList<int> l2;
			for (auto i = 0; i != 100; ++i) {
				l1.push_back(i);
				l2.push_back(i);
			}

			l1.erase(l1.begin()); l1.erase(--l1.end());
			l2.erase(l2.begin()); l2.erase(--l2.end());
			assert(mySTL::testHelper::equal(l1, l2));

			l1.erase(l1.begin(), l1.end());
			l2.erase(l2.begin(), l2.end());
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test8() {
			myList<int> l1, l2;
			l1.push_back(1); l1.push_back(1); l1.push_back(1);
			l2.push_back(2); l2.push_back(2);

			l1.swap(l2);
			assert(l1.size() == 2 && l2.size() == 3);
		}
		void test9() {
			std::random_device rd;
			stdList<int> l1;
			myList<int> l2;
			for (auto i = 0; i != 10; ++i) {
				auto ret = rd() % 65536;
				l1.push_back(ret);
				l2.push_back(ret);
			}

			l1.sort();
			l2.sort();
			assert(mySTL::testHelper::equal(l1, l2));

			l1.sort(std::greater<int>());
			l2.sort(std::greater<int>());
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test10() {
			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdList<int> l1(std::begin(arr), std::end(arr));
			myList<int> l2(std::begin(arr), std::end(arr));

			l1.reverse();
			l2.reverse();
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test11() {
			int arr1[] = { 0, 1, 3, 5, 9 }, arr2[] = { 2, 4, 6, 7, 8 };
			stdList<double> first1(std::begin(arr1), std::end(arr1)), second1(std::begin(arr2), std::end(arr2));
			myList<double> first2(std::begin(arr1), std::end(arr1)), second2(std::begin(arr2), std::end(arr2));

			first1.merge(second1);
			first2.merge(second2);
			assert(mySTL::testHelper::equal(first1, first2));
		}
		void test12() {
			int arr[] = { 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 11 };
			stdList<int> l1(std::begin(arr), std::end(arr));
			myList<int> l2(std::begin(arr), std::end(arr));

			l1.unique();
			l2.unique();
			assert(mySTL::testHelper::equal(l1, l2));

			int arr1[] = { 1, 2, 12, 23, 3, 2, 51, 1, 2, 2 };
			stdList<int> l3(std::begin(arr1), std::end(arr1));
			myList<int> l4(std::begin(arr1), std::end(arr1));
			l3.unique([mod = 10](int x, int y)
			{
				return (x % mod) == (y % mod);
			});
			l4.unique([mod = 10](int x, int y)
			{
				return (x % mod) == (y % mod);
			});
			assert(mySTL::testHelper::equal(l3, l4));

		}
		void test13() {
			int arr[] = { 17, 89, 7, 14, 89, 0, 1, 4 };
			stdList<int> l1(std::begin(arr), std::end(arr));
			myList<int> l2(std::begin(arr), std::end(arr));

			l1.remove(89);
			l2.remove(89);
			assert(mySTL::testHelper::equal(l1, l2));

			auto func = [](int n) {return n % 2 == 0; };
			l1.remove_if(func);
			l2.remove_if(func);
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test14() {
			stdList<int> l1(10, 0), l3(10, 1);
			myList<int> l2(10, 0), l4(10, 1);

			l1.splice(l1.begin(), l3);
			l2.splice(l2.begin(), l4);
			assert(mySTL::testHelper::equal(l1, l2));

			auto l5 = l1;
			auto l6 = l2;
			l1.splice(l1.end(), l5, l5.begin());
			l2.splice(l2.end(), l6, l6.begin());
			assert(mySTL::testHelper::equal(l1, l2));

			auto it1 = l1.begin();
			auto it2 = l2.begin();
			for (auto i = 0; i != l1.size() / 2; ++i) {
				++it1;
				++it2;
			}
			l1.splice(it1, l5, l5.begin(), l5.end());
			l2.splice(it2, l6, l6.begin(), l6.end());
			assert(mySTL::testHelper::equal(l1, l2));
		}
		void test15() {
			myList<int> l1(10, 0), l2(10, 1), l3(10, 0);

			assert(l1 == l3);
			assert(l1 != l2);
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
		void test16() {
			assert(TestItem::getCount() == 0);
			{
				myList<TestItem> l1(10);
				l1.push_back(TestItem());
				l1.push_front(TestItem());

				myList<TestItem> l2(10);
				l2.push_back(TestItem());
				l2.push_front(TestItem());

			}
			assert(TestItem::getCount() == 0);

			const myList<int> v(10, 9);
			//const myList<int>::iterator cit2 = v.begin(); 只能用于非const容器
			myList<int>::const_iterator it = v.begin();
			//*it = 10; *it是const
			++it;
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
			test15();
			test16();
		}
	}
}