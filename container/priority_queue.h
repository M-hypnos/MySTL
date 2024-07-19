#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_
#pragma once
#include "vector.h"
#include "heap.h"
#include "../functors.h"
namespace mySTL {
	template <class T, class Container = mySTL::vector<T>,
		class Compare = mySTL::less<typename Container::value_type>>
		class priority_queue {
		public:
			typedef Container container_type;
			typedef typename  Container::size_type size_type;
			typedef typename  Container::value_type value_type;
			typedef typename  Container::reference reference;
			typedef typename Container::const_reference const_reference;
		private:
			container_type con;
			Compare comp;
		public:
			priority_queue() {}
			explicit priority_queue(const Compare& x) :comp(x) {}
			priority_queue(const Compare& x, const container_type& cont) : comp(x), con(cont) {
				mySTL::make_heap(con.begin(), con.end(), comp);
			}
			template <class InputIterator>
			priority_queue(InputIterator first, InputIterator last) : con(first, last) {
				mySTL::make_heap(con.begin(), con.end(), comp);
			}
			template <class InputIterator>
			priority_queue(InputIterator first, InputIterator last, const Compare& x) : comp(x), con(first, last) {
				mySTL::make_heap(con.begin(), con.end(), comp);
			}
			const_reference top() { return con.front(); }
			bool empty() const { return con.empty(); }
			size_type size() const { return con.size(); }

			void push(const value_type& value) {
				con.push_back(value);
				mySTL::push_heap(con.begin(), con.end(), comp);
			}

			void pop() {
				mySTL::pop_heap(con.begin(), con.end(), comp);
				con.pop_back();
			}

			void swap(priority_queue& other) {
				con.swap(other.con);
				auto tmp = comp;
				comp = other.comp;
				other.comp = tmp;
			}
	};
}

#endif // !_PRIORITY_QUEUE_H_

