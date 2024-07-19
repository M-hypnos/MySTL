#ifndef _HEAP_TEST_H_
#define _HEAP_TEST_H_
#pragma once
#include "../container/vector.h"
#include "../container/heap.h"
#include "testHelper.h"
#include <vector>
#include <algorithm>
#include <assert.h>
namespace mySTL {
	namespace heapTest {
		void test() {
			int ia[9] = { 5, 1, 24, 3, 4, 8, 1, 3, 0 };
			mySTL::vector<int> ivec(ia, ia+9);
			mySTL::make_heap(ivec.begin(), ivec.end());

			std::vector<int> sivec(ia, ia + 9);
			std::make_heap(sivec.begin(), sivec.end());

			assert(mySTL::testHelper::equal(ivec, sivec));

			ivec.push_back(7);
			sivec.push_back(7);
			mySTL::push_heap(ivec.begin(), ivec.end());
			std::push_heap(sivec.begin(), sivec.end());
			
			assert(mySTL::testHelper::equal(ivec, sivec));

			mySTL::pop_heap(ivec.begin(), ivec.end());
			std::pop_heap(sivec.begin(), sivec.end());
			ivec.pop_back();
			sivec.pop_back();
			assert(mySTL::testHelper::equal(ivec, sivec));

			mySTL::sort_heap(ivec.begin(), ivec.end());
			std::sort_heap(sivec.begin(), sivec.end());

			assert(mySTL::testHelper::equal(ivec, sivec));
		}
	}
}
#endif // !_HEAP_TEST_H_


