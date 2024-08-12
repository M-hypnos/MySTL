#pragma once
#ifndef _RB_TREE_TEST_H_
#define _RB_TREE_TEST_H_
#include "../container/rb_tree.h"
namespace mySTL {
	namespace rbTreeTest {
		template<class Key, class Value, class KeyOfValue, class Compare>
		using rbTree = mySTL::rb_tree<Key, Value, KeyOfValue, Compare>;

		void test1();
		void test2();
		void test3();

		void test();
	}
}
#endif // !_RB_TREE_TEST_H_


