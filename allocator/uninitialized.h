#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_
#include <memory>
#include "../iterator/iterator.h"

#pragma once
namespace mySTL {
	template<class iterator, class size, class T>
	inline iterator fill_n(iterator first, size n, const T& value) {

	}

	template<class iterator, class size, class T>
	inline iterator uninitialized_fill_n_aux(iterator first, size n, const T& value) {
		return std::fill_n(first, n, value);
	}

	template<class iterator, class size, class T>
	inline iterator uninitialized_fill_n(iterator first, size n, const T& value) {
		using value_type = typename mySTL::iterator_traits<iterator>::value_type;
		return uninitialized_fill_n_aux(first, n, value);
	}
}

#endif // !_UNINITIALIZED_H_

