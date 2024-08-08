#include "vectorTest.h"
#include <string>
#include "../allocator/allocator.h"
#include "testHelper.h"

#include <assert.h>
#include <array>
#include <iostream>

namespace mySTL {
	namespace vectorTest {
		void test1() {
			stdVec<std::string> v1(10, "1");
			myVec<std::string> v2(10, "1");

			assert(mySTL::testHelper::equal(v1, v2));

			stdVec<std::string> v3(10);
			myVec<std::string> v4(10);

			assert(mySTL::testHelper::equal(v3, v4));

			std::array<std::string, 3> arr = { "abc", "def", "ghi" };
			stdVec<std::string> v5(std::begin(arr), std::end(arr));
			myVec<std::string> v6(std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal(v5, v6));
		}
		void test2() {
			stdVec<int> temp1(10, 0);
			myVec<int> temp2(10, 0);
			auto v1(temp1);
			auto v2(temp2);
			assert(mySTL::testHelper::equal(v1, v2));

			auto v3(std::move(temp1));
			auto v4(std::move(temp2));
			assert(mySTL::testHelper::equal(v3, v4));

			auto v5 = v1;
			auto v6 = v2;
			assert(mySTL::testHelper::equal(v5, v6));

			auto v7 = std::move(v3);
			auto v8 = std::move(v4);
			assert(mySTL::testHelper::equal(v7, v8));

			v7.insert(v7.begin(), 1, 1);
			v8.insert(v8.begin(), 1, 1);
			assert(mySTL::testHelper::equal(v7, v8));
		}

		void test3() {
			myVec<int> v1, v2;
			for (int i = 0; i != 100; ++i) {
				v1.push_back(i);
				v2.push_back(i);
			}

			assert(v1 == v2);
			assert(!(v1 != v2));
		}
		void test4() {
			myVec<int> myvector;
			for (int i = 1; i <= 5; i++) myvector.push_back(i);

			auto i = 1;
			for (myVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, ++i) {
				assert(*it == i);
			}

			i = 1;
			for (myVec<int>::const_iterator it = myvector.cbegin(); it != myvector.cend(); ++it, ++i) {
				assert(*it == i);
			}
		}
		void test5() {
			myVec<int> myvector(5);  // 5 default-constructed ints
			int i = 0;
			myVec<int>::reverse_iterator rit = myvector.rbegin();
			for (; rit != myvector.rend(); ++rit)
				*rit = ++i;

			i = 5;
			for (myVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, --i) {
				assert(*it == i);
			}

			i = 1;
			for (myVec<int>::reverse_iterator it = myvector.rbegin(); it != myvector.rend(); ++it, ++i) {
				assert(*it == i);
			}
		}
		void test6() {
			myVec<int> v(11, 0);
			assert(v.size() == 11);

			v.resize(5);
			assert(v.size() == 5);

			v.resize(20);
			assert(v.size() == 20);
		}
		void test7() {
			myVec<int> v;
			v.reserve(20);
			assert(v.capacity() == 20);
		}
		void test8() {
			stdVec<int> v1(10);
			myVec<int> v2(10);
			for (unsigned i = 0; i < 10; i++) {
				v1[i] = i;
				v2[i] = i;
			}
			assert(mySTL::testHelper::equal(v1, v2));

			v1.front() = 99;
			v2.front() = 99;
			v1.back() = 100;
			v2.back() = 100;

			assert(mySTL::testHelper::equal(v1, v2));
		}
		void test9() {
			stdVec<int> v1(5);
			myVec<int> v2(5);

			auto p1 = v1.data();
			auto p2 = v2.data();
			*p1 = 10; ++p1; *p1 = 20; p1[2] = 100;
			*p2 = 10; ++p2; *p2 = 20; p2[2] = 100;

			assert(mySTL::testHelper::equal(v1, v2));
		}
		void test10() {
			myVec<int> foo(3, 100);
			myVec<int> bar(2, 200);

			stdVec<int> v1 = { 100, 100, 100 };
			stdVec<int> v2 = { 200, 200 };
			assert(mySTL::testHelper::equal(foo, v1));
			assert(mySTL::testHelper::equal(bar, v2));

			foo.swap(bar);
			assert(mySTL::testHelper::equal(bar, v1));
			assert(mySTL::testHelper::equal(foo, v2));
		}
		void test11() {
			stdVec<std::string> v1;
			myVec<std::string> v2;

			v1.push_back("hello "); v1.push_back("world");
			v2.push_back("hello "); v2.push_back("world");
			assert(mySTL::testHelper::equal(v1, v2));

			v1.pop_back();
			v2.pop_back();
			assert(mySTL::testHelper::equal(v1, v2));
		}
		void test12() {
			stdVec<int> v1;
			myVec<int> v2;

			v1.insert(v1.begin(), 0);
			v2.insert(v2.begin(), 0);
			assert(mySTL::testHelper::equal(v1, v2));

			v1.insert(v1.end(), 1);
			v2.insert(v2.end(), 1);
			assert(mySTL::testHelper::equal(v1, v2));

			v1.insert(v1.begin() + v1.size() / 2, 10, 0);
			v2.insert(v2.begin() + v2.size() / 2, 10, 0);
			assert(mySTL::testHelper::equal(v1, v2));

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			v1.insert(v1.end(), std::begin(arr), std::end(arr));
			v2.insert(v2.end(), std::begin(arr), std::end(arr));
			assert(mySTL::testHelper::equal(v1, v2));
		}
		void test13() {
			stdVec<int> v1;
			myVec<int> v2;
			for (int i = 1; i <= 10; i++) {
				v1.push_back(i);
				v2.push_back(i);
			}
			v1.erase(v1.begin() + 5);
			v2.erase(v2.begin() + 5);
			assert(mySTL::testHelper::equal(v1, v2));

			v1.erase(v1.begin(), v1.begin() + 3);
			v2.erase(v2.begin(), v2.begin() + 3);
			assert(mySTL::testHelper::equal(v1, v2));
		}
		void test14() {
			myVec<int> foo(3, 100);
			myVec<int> bar(2, 200);

			assert(!(foo == bar));
			assert(foo != bar);
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

		void test15() {
			assert(TestItem::getCount() == 0);
			{
				typedef mySTL::vector<TestItem> TVector;
				TVector t(10);
				t.push_back(TestItem());
				t.push_back(TestItem());
				t.push_back(TestItem());
				t.insert(t.begin(), t.begin(), t.begin() + 1);

				typedef mySTL::vector<int> mVector;
				mVector tv(10);
				tv.push_back(1);
				tv.push_back(1);
				tv.push_back(1);
				tv.insert(tv.begin(), tv.begin(), tv.begin() + 1);

			}
			assert(TestItem::getCount() == 0);


			const myVec<int> v(10, 9);
			//const myVec<int>::iterator cit2 = v.begin(); 只能用于非const容器
			myVec<int>::const_iterator it = v.begin();
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
		}
	}
}