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
				return size < 512 ? static_cast<size_type>(512 / size) : static_cast<size_type>(1);
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
				last = *ptr + static_cast<difference_type>(buffer_size());
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
					if (cur + n < first) {
						node_offset = -((first - cur - n) / static_cast<difference_type>(buffer_size()) + 1);
					}
					else {
						node_offset = (cur + n - last + 1) / static_cast<difference_type>(buffer_size()) + 1;
					}
					difference_type offset = cur - first;
					setNode(node + node_offset);

					if (node_offset > 0) {
						cur = first + (n + offset) % static_cast<difference_type>(buffer_size());
					}
					else {
						cur = last + (n + offset) % static_cast<difference_type>(buffer_size());
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
			iterator begin() const { return start; }
			iterator end() { return finish; }
			iterator end() const { return finish; }

			reference front() { return *start; }
			reference back() { return *(finish-1); }

			size_type size() const;
			bool empty() const{ return size() == 0; }

			void push_front(const value_type& val);
			void push_back(const value_type& val);
			void pop_front();
			void pop_back();


			iterator insert(iterator position, const value_type& val);

			void swap(deque& x);

			template<class T, class Alloc>
			friend void swap(deque<T, Alloc>& lhs, deque<T, Alloc>& rhs);

		protected:
			void create_map_nodes(size_type n);

			template<class InputIterator>
			void deque_aux(InputIterator first, InputIterator last, std::false_type);
			template<class Interger>
			void deque_aux(size_type n, Interger val, std::true_type);

			iterator reserve_elements_at_front(size_type n);
			void reserve_map_at_front(size_type add_nodes_num);
			iterator reserve_elements_at_back(size_type n);
			void reserve_map_at_back(size_type add_nodes_num);
			void reallocate_map(size_type add_nodes_num, bool is_front);
			
			void destroy_element(pointer ptr);
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
		std::false_type type;
		deque_aux(other.begin(), other.end(), type);
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
		difference_type node_diff = finish.node - start.node;
		return (node_diff + 1) * buffer_size - (start.cur - start.first) - (finish.last - finish.cur);
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::reserve_elements_at_front(size_type n) {
		size_type surplus_n = start.cur - start.first;
		if (surplus_n <= n) {
			return start - difference_type(n);
		}
		else {
			reserve_map_at_front(size_type((n - surplus_n) / buffer_size + 1));
		}
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
		if (surplus_n <= n) {
			return finish + difference_type(n);
		}
		else {
			reserve_map_at_back(size_type((n - surplus_n) / buffer_size + 1));
		}
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
			startMapPtr = newMap + (map_size - new_nodes_num) / 2;
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
			mySTL::uninitialized_fill_n(start.cur-1, 1, val);
			start--;
		}
		else {
			reserve_map_at_front(1);
			*(start.node-1) = Alloc::allocate(buffer_size);
			start--;
			start.cur = start.last - 1;
			mySTL::uninitialized_fill_n(start.cur, 1, val);
		}
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::pop_front() {
		Alloc::destroy(start.cur);
		start++;
	}
	
	template<class T, class Alloc>
	void deque<T, Alloc>::push_back(const value_type& val) {
		if (finish.cur != finish.last) {
			mySTL::uninitialized_fill_n(finish.cur, 1, val);
			finish++;
		}
		else {
			reserve_map_at_back(1);
			*(finish.node + 1) = Alloc::allocate(buffer_size);
			finish++;
			finish.cur = finish.first;
			mySTL::uninitialized_fill_n(finish.cur, 1, val);
		}
	}

	template<class T, class Alloc>
	void deque<T, Alloc>::pop_back() {
		finish--;
		Alloc::destroy(finish.cur);
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, const value_type& val) {
		
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
}


#endif // !_DEQUE_H_

