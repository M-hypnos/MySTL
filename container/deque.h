#ifndef _DEQUE_H_
#define _DEQUE_H_
#pragma once

#include "../allocator/allocator.h"
#include "../iterator/iterator.h"
#include "../iterator/reverseIterator.h"
#include "../allocator/construct.h"
#include "../allocator/uninitialized.h"

namespace mySTL {
	namespace deque_detial {
		template<class T>
		struct deque_iterator
		{
			typedef random_access_iterator_tag iterator_category;
			typedef T value_type;
			typedef ptrdiff_t difference_type;
			typedef size_t size_type;
			typedef T* pointer;
			typedef T& reference;
			typedef T** mapPtr;
			
			pointer cur;
			pointer first;
			pointer last;
			mapPtr node;

			deque_iterator() = default;
			explicit deque_iterator(pointer c, pointer f, pointer l, mapPtr n) : cur(c), first(f), last(f), node(n) {}
			deque_iterator(const deque_iterator& x) :cur(x.cur), first(x.first), last(x.last), node(x.node) {}

			constexpr static size_type buffer_size() {
				size_type size = sizeof(T);
				return size < 512 ? size_type(512 / size) : size_type(1);
			}

			reference operator*() const { return *cur; }
			pointer operator->() const { return cur; }

			deque_iterator operator++(int) {
				deque_iterator tmp = *this;
				++*this;
				return tmp;
			}
			deque_iterator operator--(int) {
				deque_iterator tmp = *this;
				--*this;
				return tmp;
			}
			deque_iterator& operator++() {
				cur++;
				if (cur == last) {
					setNode(node + 1);
					cur = first;
				}
				return *this;
			}
			deque_iterator& operator--() {
				if (cur == first) {
					setNode(node - 1);
					cur = last;
				}
				cur--;
				return *this;
			}
			void setNode(mapPtr ptr) {
				node = ptr;
				first = *ptr;
				last = *ptr + difference_type(buffer_size());
			}
			bool operator== (const deque_iterator& rhs) const {
				return cur == rhs.cur;
			}
			bool operator!= (const deque_iterator& rhs) const {
				return !(cur == rhs.cur);
			}

			deque_iterator& operator+= (difference_type n) {
				if (cur + n < last && cur + n >= first) {
					cur += n;
				}
				else {
					difference_type node_offset = 0;
					if (n < 0) {
						node_offset = -((-n - (cur - first) - 1) / difference_type(buffer_size()) + 1);
					}
					else {
						node_offset = (cur - first + n) / difference_type(buffer_size());
					}
					difference_type offset = cur - first;
					setNode(node + node_offset);
					if (node_offset < 0) {
						cur = first + (-node_offset) * difference_type(buffer_size()) + offset - (-n);
					}
					else {
						cur = first + (offset + n) % difference_type(buffer_size());
					}
				}
				return *this;
			}

			deque_iterator operator+ (difference_type n) const {
				auto tmp = *this;
				return tmp += n;
			}

			deque_iterator& operator-=(difference_type n) {
				return *this += (-n);
			}

			deque_iterator operator-(difference_type n) const {
				auto tmp = *this;
				return tmp -= n;
			}

			difference_type operator-(const deque_iterator& other) const {
				return (node - other.node) * buffer_size() + (cur - first) - (other.cur - other.first);
			}
		};
	}

	template<class T, class Alloc = Allocator<T>>
	class deque {
		public:
			typedef size_t size_type;
			typedef T value_type;
			typedef ptrdiff_t difference_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef T* pointer;
			typedef T** mapPtr;
			typedef deque_detial::deque_iterator<T> iterator;
			typedef const deque_detial::deque_iterator<T> const_iterator;
			typedef mySTL::reverse_iterator<iterator> reverse_iterator;
			typedef mySTL::reverse_iterator<const_iterator> const_reverse_iterator;
			constexpr static size_type buffer_size = iterator::buffer_size();
		protected:
			typedef Allocator<pointer> mapAlloc;
			iterator start;
			iterator finish;
			mapPtr map;
			size_type map_size;
		public:
			deque();
			explicit deque(size_type n, const value_type& val = value_type());
			template< class InputIterator >
			deque(InputIterator first, InputIterator last);

			deque(const deque& other);
			deque& operator=(const deque& other);
			deque(deque&& other);
			deque& operator=(deque&& other);

			~deque();

			reference operator[] (const difference_type& n) { return *(start + n); }

			iterator begin() { return start; }
			iterator end() { return finish; }
			iterator begin() const { return start; }
			iterator end() const { return finish; }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }


			reference front() { return *start; }
			reference back() { return *(finish-1); }

			size_type size() const;
			bool empty() const{ return size() == 0; }

			void push_front(const value_type& val);
			void push_back(const value_type& val);
			void pop_front();
			void pop_back();

			void clear();

			iterator erase(iterator position);
			iterator erase(iterator first, iterator last);

			iterator insert(iterator position, const value_type& val);
			iterator insert(iterator position, size_type n, const value_type& val);
			template<class InputIterator>
			iterator insert(iterator position, InputIterator first, InputIterator last);

			void swap(deque& x);
			void resize(size_type n, const value_type value = value_type());

			reference at(size_type pos);
			reference operator[](size_type pos) { return at(pos); }

			template<class T, class Alloc>
			friend void swap(deque<T, Alloc>& lhs, deque<T, Alloc>& rhs);

			template<class T, class Alloc>
			friend bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);

			template<class T, class Alloc>
			friend bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);

		protected:
			void create_map_nodes(size_type n);

			template<class InputIterator>
			void deque_aux(InputIterator first, InputIterator last, std::false_type);
			template<class Interger>
			void deque_aux(size_type n, Interger val, std::true_type);

			template<class InputIterator>
			iterator insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
			template<class Interger>
			iterator insert_aux(iterator position, size_type n, Interger val, std::true_type);

			iterator reserve_elements_at_front(size_type n);
			void reserve_map_at_front(size_type add_nodes_num);
			iterator reserve_elements_at_back(size_type n);
			void reserve_map_at_back(size_type add_nodes_num);
			void reallocate_map(size_type add_nodes_num, bool is_front);
			
	};

	template<class T, class Alloc>
	void deque<T, Alloc>::create_map_nodes(size_type n) {
		size_type nodeNums = n / buffer_size + 1;
		map_size = std::max(size_type(8), nodeNums + 2);
		map = mapAlloc::allocate(map_size);
		mapPtr mStart = map + (map_size - nodeNums) / 2;
		mapPtr mFinish = mStart + nodeNums - 1;
		for (mapPtr mCur = mStart; mCur <= mFinish; mCur++) {
			*mCur = Alloc::allocate(buffer_size);
		}
		start.setNode(mStart);
		finish.setNode(mFinish);
		start.cur = start.first;
		finish.cur = finish.first + n % buffer_size;
	}
	
	template<class T, class Alloc>
	deque<T, Alloc>::deque(size_type n, const value_type& val) {
		std::true_type type;
		deque_aux(n, val, type);
	}
	
	template<class T, class Alloc>
	template< class InputIterator >
	deque<T, Alloc>::deque(InputIterator first, InputIterator last){
		deque_aux(first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void deque<T, Alloc>::deque_aux(InputIterator first, InputIterator last, std::false_type) {
		create_map_nodes(0);
		for (auto iter = first; iter != last; iter++) {
			push_back(*iter);
		}
	}

	template<class T, class Alloc>
	template<class Interger>
	void deque<T, Alloc>::deque_aux(size_type n, Interger val, std::true_type) {
		create_map_nodes(n);
		for (mapPtr cur = start.node; cur < finish.node; cur++) {
			mySTL::uninitialized_fill_n(*cur, buffer_size, val);
		}
		mySTL::uninitialized_fill_n(finish.first, n % buffer_size, val);
	}

	template<class T, class Alloc>
	deque<T, Alloc>::deque() {
		create_map_nodes(0);
	}

	template<class T, class Alloc>
	deque<T, Alloc>::deque(const deque& other) {
		std::false_type type;
		deque_aux(other.begin(), other.end(), type);
	}
	
	template<class T, class Alloc>
	deque<T, Alloc>::deque(deque&& other) {
		create_map_nodes(0);
		swap(other);
	}

	template<class T, class Alloc>
	deque<T, Alloc>&  deque<T, Alloc>::operator=(const deque& other) {
		if (*this == other) return *this;
		if (size() > other.size()) {
			std::copy(other.start, other.finish, start);
			erase(start + other.size(), finish);
		}
		else {
			std::copy(other.start, other.start + size(), start);
			insert(finish, other.start + size(), other.finish);
		}
		return *this;
	}
	
	template<class T, class Alloc>
	deque<T, Alloc>&  deque<T, Alloc>::operator=(deque&& other) {
		if (*this == other) return *this;

		create_map_nodes(0);

		swap(other);

		return *this;
	}

	template<class T, class Alloc>
	deque<T, Alloc>::~deque() {
		auto iter = start;
		while (iter != finish) {
			Alloc::destroy(iter.cur);
			iter++;
		}
		for (auto node = start.node; node <= finish.node; node++) {
			Alloc::deallocate(*node, buffer_size);
		}
		mapAlloc::deallocate(map, map_size);
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::size_type deque<T, Alloc>::size() const {
		return finish - start;
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::reserve_elements_at_front(size_type n) {
		size_type surplus_n = start.cur - start.first;
		if (surplus_n < n) {
			size_type add_nodes_num = (n - 1 - surplus_n) / buffer_size + 1;
			reserve_map_at_front(add_nodes_num);
			for (auto i = 1; i <= add_nodes_num; i++) {
				*(start.node - i) = Alloc::allocate(buffer_size);
			}
		}
		return start - difference_type(n);
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::reserve_map_at_front(size_type add_nodes_num) {
		if (add_nodes_num + 1 > start.node - map) {
			reallocate_map(add_nodes_num, true);
		}
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::reserve_elements_at_back(size_type n) {
		size_type surplus_n = finish.last - finish.cur;
		if (surplus_n < n) {
			size_type add_nodes_num = (n - surplus_n) / buffer_size + 1;
			reserve_map_at_back(add_nodes_num);
			for (auto i = 1; i <= add_nodes_num; i++) {
				*(finish.node + i) = Alloc::allocate(buffer_size);
			}

		}
		return finish + difference_type(n);
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::reserve_map_at_back(size_type add_nodes_num) {
		if (add_nodes_num + 1 > map_size - (finish.node - map)) {
			reallocate_map(add_nodes_num, false);
		}
	}
	
	template<class T, class Alloc>
	void deque<T, Alloc>::reallocate_map(size_type add_nodes_num, bool is_front) {
		auto old_nodes_num = finish.node - start.node + 1;
		auto new_nodes_num = old_nodes_num + add_nodes_num;
		mapPtr startMapPtr;
		if (map_size > 2 * new_nodes_num) {
			startMapPtr = map + (map_size - new_nodes_num) / 2;
			if (is_front) startMapPtr += add_nodes_num;
			if (startMapPtr < start.node) {
				std::copy(start.node, finish.node + 1, startMapPtr);
			}
			else {
				std::copy_backward(start.node, finish.node + 1, startMapPtr + old_nodes_num);
			}
		}
		else {
			size_type new_map_size = std::max(2 * map_size, map_size + add_nodes_num) + 2;
			mapPtr newMap = mapAlloc::allocate(new_map_size);
			startMapPtr = newMap + (new_map_size - new_nodes_num) / 2;
			if (is_front) startMapPtr += add_nodes_num;
			std::copy(start.node, finish.node + 1, startMapPtr);
			mapAlloc::deallocate(map, map_size);
			map = newMap;
			map_size = new_map_size;
		}
		start.node = startMapPtr;
		finish.node = startMapPtr + old_nodes_num - 1;
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::push_front(const value_type& val) {
		if (start.cur != start.first) {
			start.cur--;
		}
		else {
			reserve_map_at_front(1);
			*(start.node-1) = Alloc::allocate(buffer_size);
			start--;
		}
		mySTL::uninitialized_fill_n(start.cur, 1, val);
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::pop_front() {
		Alloc::destroy(start.cur);
		start++;
		if (start.cur == start.first) {
			Alloc::deallocate(*(start.node-1), buffer_size);
		}
	}
	
	template<class T, class Alloc>
	void deque<T, Alloc>::push_back(const value_type& val) {
		mySTL::uninitialized_fill_n(finish.cur, 1, val);
		finish.cur++;
		if (finish.cur == finish.last) {
			reserve_map_at_back(1);
			*(finish.node + 1) = Alloc::allocate(buffer_size);
			finish.setNode(finish.node + 1);
			finish.cur = finish.first;
		}
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::pop_back() {
		if (finish.cur == finish.first) {
			Alloc::deallocate(finish.cur, buffer_size);
		}
		finish--;
		Alloc::destroy(finish.cur);
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator position) {
		if (position == end()) return end();
		return erase(position, position + 1);
	}
	
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator first, iterator last) {
		if (last - first == 0) return last;
		difference_type diff = finish - last;
		mySTL::uninitialized_copy(first, last, finish);
		auto new_finish = first + diff;
		for (auto node = new_finish.node + 1; node < finish.node; node++) {
			Alloc::destroy(*node, buffer_size);
			Alloc::deallocate(*node, buffer_size);
		}
		if (new_finish.node == finish.node) {
			Alloc::destroy(new_finish.cur, finish.cur - new_finish.cur);
		}
		else {
			Alloc::destroy(new_finish.cur, new_finish.last - new_finish.cur);
			Alloc::destroy(finish.first, finish.cur - finish.first);
			Alloc::deallocate(*finish.node, buffer_size);
		}
		finish = new_finish;
		return first;
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, const value_type& val) {
		if (position == start) {
			push_front(val);
			return start;
		}
		else if (position == finish) {
			push_back(val);
			return finish - 1;
		}
		else {
			difference_type len = position - start;
			if (len < size() / 2) {
				push_front(front());
				difference_type front_offset = 1;
				iterator pos = start + len;
				std::copy(start + 1 + front_offset, pos + front_offset, start+ front_offset);
				*pos = val;
				return pos;
			}
			else {
				push_back(back());
				difference_type back_offset = 1;
				iterator pos = start + len;
				std::copy_backward(pos, finish - 1 - back_offset, finish - back_offset);
				*pos = val;
				return position;
			}
		}
	}
	
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, size_type n, const value_type& val) {
		std::true_type type;
		return insert_aux(position, n, val, type);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		return insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	template<class Interger>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator position, size_type n, Interger val, std::true_type) {
		if (n == 0) return position;
		if (position == start) {
			start = reserve_elements_at_front(n);
			mySTL::uninitialized_fill_n(start, n, val);
			return start;
		}
		else if (position == finish) {
			finish = reserve_elements_at_back(n);
			mySTL::uninitialized_fill_n(finish - n, n, val);
			return finish - n;
		}
		else {
			difference_type len = position - start;
			if (len < size() / 2) {
				auto new_start = reserve_elements_at_front(n);
				auto pos = start + len;
				if (n >= len) {
					mySTL::uninitialized_copy(new_start, start, pos);
					mySTL::uninitialized_fill_n(new_start + len, n - len, val);
					std::fill_n(start, len, val);
				}
				else {
					mySTL::uninitialized_copy(new_start, start, start + n);
					std::copy(start + n, pos, start);
					std::fill_n(pos - n, n, val);
				}
				start = new_start;
			}
			else {
				auto after_len = finish - position;
				auto new_finish = reserve_elements_at_back(n);
				auto pos = finish - after_len;
				if (n >= after_len) {
					mySTL::uninitialized_copy(new_finish - after_len, pos, finish);
					mySTL::uninitialized_fill_n(finish, n - after_len, val);
					std::fill_n(pos, after_len, val);
				}
				else {
					mySTL::uninitialized_copy(finish, finish - n, finish);
					std::copy_backward(pos, finish - n, finish);
					std::fill_n(pos, n, val);
				}
				finish = new_finish;
			}
			return start + len;
		}
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		difference_type n = last - first;
		if (position == start) {
			start = reserve_elements_at_front(n);
			mySTL::uninitialized_copy(start, first, last);
			return start;
		}
		else if (position == finish) {
			finish = reserve_elements_at_back(n);
			mySTL::uninitialized_copy(finish - n, first, last);
			return finish - n;
		}
		else {
			difference_type len = position - start;
			if (len < size() / 2) {
				auto new_start = reserve_elements_at_front(n);
				auto pos = start + len;
				if (n >= len) {
					mySTL::uninitialized_copy(new_start, start, pos);
					mySTL::uninitialized_copy(new_start + len, first, last - len);
					std::copy(last - len, last, start);
				}
				else {
					mySTL::uninitialized_copy(new_start, start, start + n);
					std::copy(start + n, pos, start);
					std::copy(first, last, pos - n);
				}
				start = new_start;
			}
			else {
				auto after_len = finish - position;
				auto new_finish = reserve_elements_at_back(n);
				auto pos = finish - after_len;
				if (n >= after_len) {
					mySTL::uninitialized_copy(new_finish - after_len, pos, finish);
					mySTL::uninitialized_copy(finish, first + after_len, last);
					std::copy(first, first + after_len, pos);
				}
				else {
					mySTL::uninitialized_copy(finish, finish - n, finish);
					std::copy_backward(pos, finish - n, finish);
					std::copy(first, last, pos);
				}
				finish = new_finish;
			}
			return start + len;
		}
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::at(size_type pos) {
		auto tmp = begin();
		return *(tmp + pos);
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::clear() {
		erase(start, finish);
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::swap(deque& x) {
		std::swap(map, x.map);
		std::swap(map_size, x.map_size);
		std::swap(start, x.start);
		std::swap(finish, x.finish);
	}

	template<class T, class Alloc>
	void swap(deque<T, Alloc>& lhs, deque<T, Alloc>& rhs) {
		lhs.swap(rhs);
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::resize(size_type n, value_type value) {
		if (n == size()) return;
		if (n < size()) {
			erase(start + n, finish);
		}
		else {
			insert(finish, n - size(), value);
		}
	}

	template<class T, class Alloc>
	bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
		auto it1 = lhs.begin();
		auto it2 = rhs.begin();
		while (it1 != lhs.end() && it2 != rhs.end()) {
			if (*it1 != *it2)
				return false;
			it1++;
			it2++;
		}
		return it1 == lhs.end() && it2 == rhs.end();
	}

	template<class T, class Alloc>
	bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}
}


#endif // !_DEQUE_H_

