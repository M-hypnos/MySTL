#include "slistTest.h"
#include "testHelper.h"
#include <assert.h>
#include <random>

namespace mySTL {
	namespace slistTest {
		void test1() {
			stdSlist<int> l1(2, 0);
			mySlist<int> l2(2, 0);

			assert(mySTL::testHelper::equal_ex(l1, l2));

			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdSlist<int> l3(std::begin(arr), std::end(arr));
			mySlist<int> l4(std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal_ex(l3, l4));

			auto l5(l1);
			auto l6(l2);
			assert(mySTL::testHelper::equal_ex(l5, l6));

			stdSlist<int> l7(15, 10);
			mySlist<int> l8(15, 10);
			l7 = l1;
			l8 = l2;
			assert(mySTL::testHelper::equal_ex(l7, l8));
			stdSlist<int> l9(5, 10);
			mySlist<int> l10(5, 10);
			l9 = l1;
			l10 = l2;
			assert(mySTL::testHelper::equal_ex(l9, l10));

			l9.clear();
			l10.clear();
			assert(mySTL::testHelper::equal_ex(l9, l10));
		}
		void test2() {
			mySlist<std::string> l1;
			assert(l1.empty());

			std::string arr[] = { "1", "2", "3" };
			mySlist<std::string> l2(std::begin(arr), std::end(arr));
			assert(!l2.empty());
		}
		void test3() {
			std::string arr[] = { "1", "2", "3" };
			mySlist<std::string> l(std::begin(arr), std::end(arr));
			assert(l.front() == arr[0]);

			l.front() = "front";
			assert(l.front() == "front");
		}
		void test4() {
			stdSlist<int> l1;
			mySlist<int> l2;
			for (auto i = 0; i != 10; ++i) {
				l1.push_front(i);
				l2.push_front(i);
			}
			assert(mySTL::testHelper::equal_ex(l1, l2));
			for (auto i = 0; i != 5; ++i) {
				l1.pop_front();
				l2.pop_front();
			}
			assert(mySTL::testHelper::equal_ex(l1, l2));
		}
		void test5() {
			stdSlist<int> l1;
			mySlist<int> l2;

			int size = 10;
			l1.insert_after(l1.before_begin(), size, -1);
			l2.insert_after(l2.before_begin(), size, -1);
			assert(mySTL::testHelper::equal_ex(l1, l2));

			auto it1 = l1.begin();
			auto it2 = l2.begin();
			for (auto i = size / 2 + 1; i != 0; --i) {
				++it1;
				++it2;
			}
			l1.insert_after(it1, 1);
			l2.insert_after(it2, 1);
			assert(mySTL::testHelper::equal_ex(l1, l2));

			int arr[] = { 1, 2, 3, 4, 5 };
			it1 = l1.before_begin();
			it2 = l2.before_begin();
			l1.insert_after(it1, std::begin(arr), std::end(arr));
			l2.insert_after(it2, std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal_ex(l1, l2));
		}
		void test6() {
			stdSlist<int> l1;
			mySlist<int> l2;
			for (auto i = 0; i != 100; ++i) {
				l1.push_front(i);
				l2.push_front(i);
			}
			auto it1 = l1.begin();
			auto it2 = l2.begin();
			for (auto i = 40; i != 0; --i) {
				++it1;
				++it2;
			}
			l1.erase_after(l1.begin()); 
			l1.erase_after(it1);
			l2.erase_after(l2.begin()); 
			l2.erase_after(it2);
			assert(mySTL::testHelper::equal_ex(l1, l2));

			l1.erase_after(l1.before_begin(), l1.end());
			l2.erase_after(l2.before_begin(), l2.end());
			assert(mySTL::testHelper::equal_ex(l1, l2));
		}
		void test7() {
			mySlist<int> l1, l2;
			stdSlist<int> l3, l4;
			l1.push_front(1); l1.push_front(1); l1.push_front(1);
			l3.push_front(1); l3.push_front(1); l3.push_front(1);
			l2.push_front(2); l2.push_front(2);
			l4.push_front(2); l4.push_front(2);

			l1.swap(l2);
			assert(mySTL::testHelper::equal_ex(l1, l4));
			assert(mySTL::testHelper::equal_ex(l2, l3));
		}
		void test8() {
			std::random_device rd;
			stdSlist<int> l1;
			mySlist<int> l2;
			for (auto i = 0; i != 10; ++i) {
				auto ret = rd() % 65536;
				l1.push_front(ret);
				l2.push_front(ret);
			}

			l1.sort();
			l2.sort();
			assert(mySTL::testHelper::equal_ex(l1, l2));

			l1.sort(std::greater<int>());
			l2.sort(std::greater<int>());
			assert(mySTL::testHelper::equal_ex(l1, l2));

		}
		void test9() {
			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdSlist<int> l1(std::begin(arr), std::end(arr));
			mySlist<int> l2(std::begin(arr), std::end(arr));

			l1.reverse();
			l2.reverse();
			assert(mySTL::testHelper::equal_ex(l1, l2));
		}
		void test10() {
			int arr1[] = { 0, 1, 3, 5, 9 }, arr2[] = { 2, 4, 6, 7, 8 };
			stdSlist<double> first1(std::begin(arr1), std::end(arr1)), second1(std::begin(arr2), std::end(arr2));
			mySlist<double> first2(std::begin(arr1), std::end(arr1)), second2(std::begin(arr2), std::end(arr2));

			first1.merge(second1);
			first2.merge(second2);
			assert(mySTL::testHelper::equal_ex(first1, first2));
		}
		void test11() {
			int arr[] = { 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 11 };
			stdSlist<int> l1(std::begin(arr), std::end(arr));
			mySlist<int> l2(std::begin(arr), std::end(arr));

			l1.unique();
			l2.unique();
			assert(mySTL::testHelper::equal_ex(l1, l2));

			int arr1[] = { 1, 2, 12, 23, 3, 2, 51, 1, 2, 2 };
			stdSlist<int> l3(std::begin(arr1), std::end(arr1));
			mySlist<int> l4(std::begin(arr1), std::end(arr1));
			l3.unique([mod = 10](int x, int y)
			{
				return (x % mod) == (y % mod);
			});
			l4.unique([mod = 10](int x, int y)
			{
				return (x % mod) == (y % mod);
			});
			assert(mySTL::testHelper::equal_ex(l3, l4));
		}
		void test12() {
			int arr[] = { 17, 89, 7, 14, 89, 0, 1, 4 };
			stdSlist<int> l1(std::begin(arr), std::end(arr));
			mySlist<int> l2(std::begin(arr), std::end(arr));

			l1.remove(89);
			l2.remove(89);
			assert(mySTL::testHelper::equal_ex(l1, l2));

			auto func = [](int n) {return n % 2 == 0; };
			l1.remove_if(func);
			l2.remove_if(func);
			assert(mySTL::testHelper::equal_ex(l1, l2));
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
		void test13() {
			assert(TestItem::getCount() == 0);
			{
				mySlist<TestItem> l1(10);
				l1.push_front(TestItem());
				l1.push_front(TestItem());
				l1.push_front(TestItem());
				l1.push_front(TestItem());
				l1.push_front(TestItem());
				l1.push_front(TestItem());

				auto it = l1.begin();
				it++;
				it++;
				it++;
				l1.insert_after(l1.begin(), l1.begin(), it);

				mySlist<TestItem> l2(10);
				l2.push_front(TestItem());
				l2.push_front(TestItem());
				l2.push_front(TestItem());
				l2.push_front(TestItem());
				l2.push_front(TestItem());
				l2.push_front(TestItem());
				it = l2.before_begin();
				it++;
				it++;
				it++;
				l2.insert_after(l2.before_begin(), l2.begin(), it);

			}
			assert(TestItem::getCount() == 0);
		}
		void test14() {
			stdSlist<int> l1;
			mySlist<int> l2;
			l1.resize(6);
			l2.resize(6);
			assert(mySTL::testHelper::equal_ex(l1, l2));
			l1.resize(2, 5);
			l2.resize(2, 5);
			assert(mySTL::testHelper::equal_ex(l1, l2));
			l1.resize(8, 7);
			l2.resize(8, 7);
			assert(mySTL::testHelper::equal_ex(l1, l2));

			const mySlist<int> v(10, 9);
			//const mySlist<int>::iterator cit2 = v.begin(); 只能用于非const容器
			mySlist<int>::const_iterator it = v.begin();
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
		}
	}
}