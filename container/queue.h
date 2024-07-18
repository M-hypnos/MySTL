#ifndef _QUEUE_H_
#define _QUEUE_H_
#pragma once
#include "deque.h"
namespace mySTL {
	template<class T, class Container = deque<T>>
	class queue {
		public:
			typedef Container container_type;
			typedef typename  Container::size_type size_type;
			typedef typename  Container::value_type value_type;
			typedef typename  Container::reference reference;
			typedef typename  Container::const_reference const_reference;
		private:
			container_type con;
		public:
			queue() {}
			explicit queue(const container_type& cont):con(cont){}

			reference front() { return con.front(); }
			const_reference front() const { return con.front(); }
			reference back() { return con.back(); }
			const_reference back() const { return con.back(); }
			bool empty() const { return con.empty(); }
			size_type size() const { return con.size(); }
			void push(const value_type& value) { con.push_back(value); }
			void pop() { con.pop_front(); }
			void swap(queue& other) { con.swap(other.con); }

			template <class T, class Container>
			friend bool operator== (const queue<T, Container>& lhs, const queue<T, Container>& rhs);
			template <class T, class Container>
			friend bool operator!= (const queue<T, Container>& lhs, const queue<T, Container>& rhs);
	};
	template <class T, class Container>
	bool operator== (const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
		return lhs.con == rhs.con;
	}
	template <class T, class Container>
	bool operator!= (const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
		return lhs.con != rhs.con;
	}
}

#endif // !_QUEUE_H_

