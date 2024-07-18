#ifndef _SLIST_TEST_H_
#define _SLIST_TEST_H_
#include <forward_list>
#include "../container/slist.h"
#pragma once
namespace mySTL {
	namespace slistTest {
		template<class T>
		using stdSlist = std::forward_list<T>;

		template<class T>
		using mySlist = mySTL::slist<T>;

		void test1();
		void test2();
		void test3();
		void test4();
		void test5();
		void test6();
		void test7();
		void test8();
		void test9();
		void test10();
		void test11();
		void test12();
		void test13();
		void test14();

		void test();
	}
}
#endif // !_LIST_TEST_H_


