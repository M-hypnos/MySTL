#pragma once
#ifndef _MAP_TEST_H_
#define _MAP_TEST_H_
#include "../container/map.h"
#include <map>

namespace mySTL {
	namespace mapTest {
		template<class Key, class T>
		using stdMap = std::map<Key, T>;

		template<class Key, class T>
		using myMap = mySTL::map<Key, T>;

		void test1();
		void test2();
		void test3();
		void test4();
		void test5();

		void test();
	}
}

#endif // !_MAP_TEST_H_
