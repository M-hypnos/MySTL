#ifndef _TEST_HELPER_
#define _TEST_HELPER_
#pragma once

#include <iterator>
#include <iostream>

namespace mySTL {
	namespace testHelper {
		template<class container1, class container2>
		bool equal(container1& con1, container2& con2) {
			if (con1.size() != con2.size()) 
				return false;
			auto first1 = std::begin(con1);
			auto first2 = std::begin(con2);
			while (first1 != std::end(con1)) {
				if (*first1 != *first2) {
					return false;
				}
				first1++;
				first2++;
			}
			return true;
		}

	}
}

#endif // !_TEST_HELPER_

