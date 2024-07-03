#ifndef _VECTOR_H
#define _VECTOR_H

#include "../allocator/allocator.h"
#include "../allocator/uninitialized.h"
#include "../iterator/reverseIterator.h"
#include <algorithm>
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
			typedef T* iterator;
			typedef ptrdiff_t difference_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef T* pointer;

			typedef const T* const_iterator;
			typedef mySTL::reverse_iterator<iterator> reverse_iterator;
			typedef mySTL::reverse_iterator<const_iterator> const_reverse_iterator;
		public:
			vector():start(0), finish(0), end_of_storage(0) {}
			explicit vector(const size_type n);
			vector(const size_type n, const value_type& value);
			template<class InputIterator>
			vector(InputIterator first, InputIterator last);
			vector(const vector& v);
			vector(vector&& v);

			vector& operator=(const vector& b);
			vector& operator=(vector&& b);

			~vector();

			iterator begin() { return start; }
			const_iterator begin() const { return start; }
			const_iterator cbegin() const { return start; }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator crbegin() const {return const_reverse_iterator(end()); }
			iterator end() { return finish; }
			const_iterator end() const { return finish; }
			const_iterator cend() const { return finish; }
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

			difference_type size() const { return finish - start; }
			difference_type capacity() const { return end_of_storage - start; }

			bool empty() { return start == finish; }

			void resize(size_type n, value_type value = value_type());
			void reserve(size_type n);
			void shrink_to_fit();

			void push_back(const value_type& value);
			void pop_back();

			iterator insert(iterator position, const value_type& value);
			iterator insert(iterator position, size_type n, const value_type& value);
			template<class InputIterator>
			iterator insert(iterator position, InputIterator first, InputIterator last);

			iterator erase(iterator position);
			template<class InputIterator>
			iterator erase(InputIterator first, InputIterator last);

			bool operator== (const vector& v)const;
			bool operator!= (const vector& v)const;

			reference operator[] (const difference_type& n) { return *(start + n); }
			const_reference operator[] (const difference_type& n) const { return *(start + n); }
			reference front() { return *start; }
			reference back() { return *(finish - 1); }
			pointer data() { return start; }

			void swap(vector& v);
			void clear();

			template<class T, class Alloc>
			friend bool operator== (const vector& lhs, const vector& rhs);
			template<class T, class Alloc>
			friend bool operator!= (const vector& lhs, const vector& rhs);
		private:
			void allocate_and_fill(size_type n, const value_type& value);
			template<class InputIterator>
			void allocate_and_copy(InputIterator first, InputIterator last);
			void destroy_and_deallocate();

			template<class InputIterator>
			void vector_aux(InputIterator first, InputIterator last, std::false_type);
			template<class Integer>
			void vector_aux(Integer n, const value_type& value, std::true_type);

			template<class InputIterator>
			iterator insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
			template<class Integer>
			iterator insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
	};

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n) {
		allocate_and_fill(n, value_type());
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value) {
		allocate_and_fill(n, value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		vector_aux(first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
		allocate_and_copy(first, last);
	}

	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		allocate_and_fill(n, value);
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v) {
		allocate_and_copy(v.start, v.finish);
	}
	
	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector&& v) {
		if (this == &v) return;
		start = v.start;
		finish = v.finish;
		end_of_storage = v.end_of_storage;
		v.start = v.finish = v.end_of_storage = 0;
	}

	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v) {
		if (this == &v) return *this;
		allocate_and_copy(v.start, v.finish);
		return *this;
	}
	
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& v) {
		if (this == &v) return *this;
		start = v.start;
		finish = v.finish;
		end_of_storage = v.end_of_storage;
		v.start = v.finish = v.end_of_storage = 0;
		return *this;
	}

	template<class T, class Alloc>
	vector<T, Alloc>::~vector() {
		destroy_and_deallocate();
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::allocate_and_fill(size_type n, const value_type& value) {
		start = Alloc::allocate(n);
		mySTL::uninitialized_fill_n(start, n, value);

		finish = end_of_storage = start + n;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
		start = Alloc::allocate(last - first);
		finish = mySTL::uninitialized_copy(start, first, last);
		end_of_storage = finish;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::destroy_and_deallocate() {
		if (capacity() == 0) return;
		Alloc::destroy(start, size());
		Alloc::deallocate(start, capacity());
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type& value) {
		insert(end(), 1, value);
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		finish--;
		Alloc::destroy(finish);
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n, value_type value) {
		if (n == size()) return;
		if (n < size()) {
			Alloc::destroy(start + n, size() - n);
			finish = start + n;
		}
		else if (n > size() && n <= capacity()) {
			finish = mySTL::uninitialized_fill_n(finish, n - size(), value);
		}
		else {
			const size_type newSize = 2 * capacity() > n ? 2 * capacity() : n + capacity();
			T* newStart = Alloc::allocate(newSize);
			T* newFinish = mySTL::uninitialized_copy(newStart, start, finish);
			newFinish = mySTL::uninitialized_fill_n(newFinish, n - size(), value);
			destroy_and_deallocate();
			start = newStart;
			finish = newFinish;
			end_of_storage = newStart + newSize;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		if (n > capacity()) {
			T* newStart = Alloc::allocate(n);
			T* newFinish = mySTL::uninitialized_copy(newStart, start, finish);
			destroy_and_deallocate();
			start = newStart;
			finish = newFinish;
			end_of_storage = newStart + n;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		if (end_of_storage > finish) {
			Alloc::deallocate(finish, end_of_storage - finish);
			end_of_storage = finish;
		}
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& value) {
		return insert(position, 1, value);
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
		return insert_aux(position, n, value, std::is_integral<size_type>::type());
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		return insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		difference_type n = last - first;
		if (n == 0) return position;
		if (size() + n < capacity()) {
			mySTL::uninitialized_copy(finish, finish - n, finish);
			std::copy_backward(position, finish - n, finish);
			std::copy(first, last, position);
			finish = finish + n;
			return position;
		}
		else {
			const size_type newSize = 2 * capacity() > size() + n ? 2 * capacity() : size() + n;
			T* newStart = Alloc::allocate(newSize);
			T* insertPos = mySTL::uninitialized_copy(newStart, start, position);
			T* newFinish = insertPos;
			newFinish = mySTL::uninitialized_copy(newFinish, first, last);
			newFinish = mySTL::uninitialized_copy(newFinish, position, finish);
			destroy_and_deallocate();
			start = newStart;
			finish = newFinish;
			end_of_storage = newStart + newSize;
			return insertPos;
		}
	}

	template<class T, class Alloc>
	template<class Integer>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type) {
		if (n == 0) return position;
		if (size() + n < capacity()) {
			std::copy_backward(position, finish, finish + n);
			mySTL::uninitialized_fill_n(position, n, value);
			finish = finish + n;
			return position;
		}
		else {
			const size_type newSize = 2 * capacity() > size() + n ? 2 * capacity() : size() + n;
			T* newStart = Alloc::allocate(newSize);
			T* insertPos = mySTL::uninitialized_copy(newStart, start, position);
			T* newFinish = insertPos;
			newFinish = mySTL::uninitialized_fill_n(newFinish, n, value);
			newFinish = mySTL::uninitialized_copy(newFinish, position, finish);
			destroy_and_deallocate();
			start = newStart;
			finish = newFinish;
			end_of_storage = newStart + newSize;
			return insertPos;
		}
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
		if (position == finish) return finish;
		return erase(position, position + 1);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(InputIterator first, InputIterator last) {
		if (last - first == 0) return last;
		if (last > finish) last = finish;
		difference_type diff = finish - last;
		mySTL::uninitialized_copy(first, last, finish);
		Alloc::destroy(first + diff, last - first);
		finish = first + diff;
		return finish;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v) {
		auto tempStart = v.start;
		auto tempFinish = v.finish;
		auto tempEndOfStorage = v.end_of_storage;
		v.start = start;
		v.finish = finish;
		v.end_of_storage = end_of_storage;
		start = tempStart;
		finish = tempFinish;
		end_of_storage = tempEndOfStorage;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::clear() {
		Alloc::destroy(start, size());
		finish = start;
	}

	template<class T, class Alloc>
	bool vector<T, Alloc>::operator==(const vector& v) const {
		if (size() != v.size()) return false;
		auto first1 = begin();
		auto first2 = v.begin();
		while (first1 != end()) {
			if (*first1 != *first2)
				return false;
			first1++;
			first2++;
		}
		return true;
	}
	
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator!=(const vector& v) const {
		return !(*this == v);
	}

	template<class T, class Alloc>
	bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lhs.operator==(rhs);
	}
	
	template<class T, class Alloc>
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lhs.operator!=(rhs);
	}
}

#endif // ! _VECTOR_H
