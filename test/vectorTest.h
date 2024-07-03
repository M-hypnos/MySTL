#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_

#include "../container/vector.h"
#include <vector>

#pragma once
namespace mySTL {
	namespace vectorTest {
		template<class T>
		using stdVec = std::vector<T>;

		template<class T>
		using myVec = mySTL::vector<T>;

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

#endif // ! _VECTOR_TEST_H_
