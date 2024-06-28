#ifndef _VECTOR_H
#define _VECTOR_H

#include "../allocator/Allocator.h"
#include "../allocator/uninitialized.h"
#pragma once

namespace mySTL {
	template<class T, class Alloc = Allocator<T>>
	class vector
	{
		private:
			T* start;
			T* finish;
			T* end_of_storage;
		public:
			typedef size_t size_type;
			typedef T value_type;
		public:
			vector():start(0), finish(0), end_of_storage(0) {}
			explicit vector(const size_type n);

		private:
			void allocate_and_fill(size_type n, const value_type& value);
	};

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n) {
		allocate_and_fill(n, value_type());
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::allocate_and_fill(size_type n, const value_type& value) {
		start = Alloc::allocate(n);
		mySTL::uninitialized_fill_n(start, n, value);

		finish = end_of_storage = start + n;
	}
}

#endif // ! _VECTOR_H
