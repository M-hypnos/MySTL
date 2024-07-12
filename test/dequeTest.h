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
		void test();
	}
}
#endif // !_DEQUE_TEST_H_


