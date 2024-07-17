#ifndef _DEQUE_TEST_H_
#define _DEQUE_TEST_H_
#pragma once
#include <deque>
#include "../container/deque.h"

namespace mySTL {
	namespace dequeTest {
		template<class T>
		using stdQueue = std::deque<T>;
		template<class T>
		using myQueue = mySTL::deque<T>;

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
#endif // !_DEQUE_TEST_H_


