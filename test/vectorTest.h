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
		using tsVec = mySTL::vector<T>;

		void test1();
	}
}

#endif // ! _VECTOR_TEST_H_
