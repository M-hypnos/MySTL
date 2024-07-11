#ifndef _LIST_TEST_H_
#define _LIST_TEST_H_
#include <list>
#include "../container/list.h"
#pragma once
namespace mySTL {
	namespace listTest {
		template<class T>
		using stdList = std::list<T>;

		template<class T>
		using myList = mySTL::list<T>;

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
		void test15();

		void test();
	}
}
#endif // !_LIST_TEST_H_


