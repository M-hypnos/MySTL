#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_
#include <memory>
#include "../iterator/iterator.h"
#include "../typeTraits.h"
#include "construct.h"

#pragma once
namespace mySTL {
	template<class iterator, class T>
	inline iterator fill_n(iterator first, size_t n, const T& value) {

	}

	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size_t n, const T& value, mySTL::_true_type) {
		return std::fill_n(first, n, value);
	}

	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size_t n, const T& value, mySTL::_false_type) {
		while (n > 0) {
			n--;
			mySTL::construct(first, value);
			first++;
		}
		return first;
	}

	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, size_t n, const T& value) {
		typedef typename mySTL::type_traits<T>::is_POD_type is_POD;
		return uninitialized_fill_n_aux(first, n, value, is_POD());
	}

	template<class ForwardIterator, class InputIterator>
	inline ForwardIterator uninitialized_copy_aux(ForwardIterator start, InputIterator first, InputIterator last, mySTL::_true_type) {
		return std::copy(first, last, start);
	}

	template<class ForwardIterator, class InputIterator>
	inline ForwardIterator uninitialized_copy_aux(ForwardIterator start, InputIterator first, InputIterator last, mySTL::_false_type) {
		while (first != last) {
			mySTL::construct(start, *first);
			first++;
			start++;
		}
		return start;
	}

	template<class ForwardIterator, class InputIterator>
	inline ForwardIterator uninitialized_copy(ForwardIterator start, InputIterator first, InputIterator last) {
		typedef typename mySTL::iterator_traits<InputIterator>::value_type value_type;
		typedef typename mySTL::type_traits <value_type> ::is_POD_type is_POD;
		return uninitialized_copy_aux(start, first, last, is_POD());
	}
}

#endif // !_UNINITIALIZED_H_

