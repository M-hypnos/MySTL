#ifndef _QUEUE_TEST_H_
#define _QUEUE_TEST_H_
#pragma once
#include <queue>
#include "../container/queue.h"

namespace mySTL {
	namespace queueTest {
		template<class T>
		using stdQueue = std::queue<T>;
		template<class T>
		using myQueue = mySTL::queue<T>;

		void test1();
		void test2();
		void test3();
		void test4();
		void test5();
		void test();
	}
}
#endif // !_QUEUE_TEST_H_


