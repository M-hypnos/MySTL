#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <new>
#pragma once
namespace mySTL {
	template<class T>
	class Allocator
	{
		public:
			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
		public:
			static T* allocate();
			static T* allocate(size_type n);
			static void deallocate(T* ptr);
			static void deallocate(T* ptr, size_type n);

	};

	template<class T>
	T* Allocator<T>::allocate() {
		return static_cast<T*>(::operator new(sizeof(T)));
	}

	template<class T>
	void Allocator<T>::deallocate(T* ptr) {
		if (ptr == nullptr) return;
		::operator delete(ptr);
	}

	template<class T>
	T* Allocator<T>::allocate(size_type n) {
		size_t size = static_cast<size_t>(sizeof(T) * n);
		return static_cast<T*>(::operator new(size));
	}

	template<class T>
	void Allocator<T>::deallocate(T* ptr, size_type n) {
		if (ptr == nullptr) return;
		while (n > 0) {
			n--;
			::operator delete(ptr);
			ptr++;
		}
	}
}

#endif // _ALLOCATOR_H_



