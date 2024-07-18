#ifndef _LIST_H_
#define _LIST_H_
#pragma once

#include "../allocator/allocator.h"
#include "../iterator/iterator.h"
#include "../iterator/reverseIterator.h"
#include "../allocator/uninitialized.h"
#include "../allocator/construct.h"
#include "../functors.h"

namespace mySTL {
	namespace list_detail {
		template<class T>
		struct list_node
		{
			list_node* prev;
			list_node* next;
			T data;
			list_node(const T& val):data(val),prev(nullptr),next(nullptr){}

			/*bool operator==(const list_node& rhs) {
				return data == rhs.data && prev == rhs.prev && next == rhs.next;
			}*/
		};

		/*template<class T, class Ref, class Ptr>
		struct list_iterator {
			typedef list_iterator<T, Ref, Ptr> self;
			typedef iterator = list_iterator<T, T&, T*>;
			typedef T value_type;
			typedef Ptr point;
			typedef Ref reference;
			typedef list_node<T>* link_type;
			typedef ptrdiff_t difference_type;
			typedef bidrectional_iterator_tag iterator_category;

			link_type node;

			list_iterator(link_type x) :node(x) {}
			list_iterator() {}
			list_iterator(const iterator& x) :node(x.node) {}


			reference operator*() const { return node->data }
			self operator++(int) { self tmp = *this; ++*this; return tmp; }
			self& operator++() { node = node->next; return *this; }

		};*/

		template<class T>
		struct list_iterator {
			typedef bidrectional_iterator_tag iterator_category;
			typedef T value_type;
			typedef ptrdiff_t difference_type;
			typedef T* pointer;
			typedef T& reference;
			typedef list_node<T>* nodePtr;

			nodePtr node;
			list_iterator():node(nullptr) {}
			explicit list_iterator(nodePtr x):node(x){}
			list_iterator(const list_iterator& x):node(x.node){}

			reference operator*() const { return node->data; }
			pointer operator->() const { return &(operator*()); }

			list_iterator operator++(int) {
				list_iterator tmp = *this;
				++*this;
				return tmp;
			}
			list_iterator operator--(int) {
				list_iterator tmp = *this;
				--*this;
				return tmp;
			}
			list_iterator& operator++() {
				node = node->next;
				return *this;
			}
			list_iterator& operator--() {
				node = node->prev;
				return *this;
			}

			template<class T>
			friend bool operator== (const list_iterator<T>& lhs, const list_iterator<T>& rhs);
			template<class T>
			friend bool operator!= (const list_iterator<T>& lhs, const list_iterator<T>& rhs);
		};

		template<class T>
		bool operator==(const list_iterator<T>& lhs, const list_iterator<T>& rhs) {
			return lhs.node == rhs.node;
		}
		template<class T>
		bool operator!=(const list_iterator<T>& lhs, const list_iterator<T>& rhs) {
			return !(lhs.node == rhs.node);
		}
	}

	template<class T, class Alloc = Allocator<list_detail::list_node<T>>>
	class list {
		public:
			typedef size_t size_type;
			typedef T value_type;
			typedef list_detail::list_iterator<T> iterator;
			typedef ptrdiff_t difference_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef T* pointer;

			typedef const iterator const_iterator;
			typedef mySTL::reverse_iterator<iterator> reverse_iterator;
			typedef mySTL::reverse_iterator<const_iterator> const_reverse_iterator;

			typedef list_detail::list_node<T>* nodePtr;
		private:
			iterator head;
			iterator tail;
		public:
			list();
			explicit list(size_type n);
			explicit list(size_type n, const value_type& val);
			list(const list& other);
			template<class InputIterator>
			list(InputIterator first, InputIterator last);
			list& operator=(const list& other);

			~list();

			iterator begin() { return head; }
			const_iterator begin() const { return head; }
			const_iterator cbegin() const { return head; }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
			iterator end() { return tail; }
			const_iterator end() const { return tail; }
			const_iterator cend() const { return tail; }
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

			reference back();
			const_reference back() const;
			reference front() { return *head; }
			const_reference front() const { return *head; }

			iterator insert(iterator position, const value_type& value);
			iterator insert(iterator position, size_type count, const value_type& value);
			template<class InputIterator>
			iterator insert(iterator position, InputIterator first, InputIterator last);

			iterator erase(iterator position);
			iterator erase(iterator first, iterator last);

			void push_front(const value_type& val);
			void push_back(const value_type& val);
			void pop_front();
			void pop_back();

			size_type size() const;
			bool empty() const { return head == tail; }

			void swap(list& other);
			void clear();

			void splice(iterator position, list& other);
			void splice(iterator position, list& other, iterator it);
			void splice(iterator position, list& other, iterator first, iterator last);

			void sort();
			template<class Compare>
			void sort(Compare comp);

			void merge(list& other);
			template<class Compare>
			void merge(list& other, Compare comp);

			void reverse();

			void unique();
			template <class BinaryPredicate>
			void unique(BinaryPredicate binary_pred);

			void remove(const T& value);
			template< class UnaryPredicate >
			void remove_if(UnaryPredicate p);

			template<class T, class Alloc>
			friend bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
			template<class T, class Alloc>
			friend bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);

		private:
			nodePtr newNode(value_type value = value_type());
			void init();

			void fill_init(size_type n, value_type value = value_type());

			void deleteNode(nodePtr ptr);

			template<class Interger>
			iterator insert_aux(iterator position, size_type count, const Interger value, std::true_type);
			template<class InputIterator>
			iterator insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
	};

	template<class T, class Alloc>
	void list<T, Alloc>::init() {
		nodePtr node = newNode();
		head.node = node;
		tail.node = node;
	}
	
	template<class T, class Alloc>
	void list<T, Alloc>::deleteNode(nodePtr ptr) {
		Alloc::destroy(ptr);
		Alloc::deallocate(ptr);
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::nodePtr list<T, Alloc>::newNode(value_type value) {
		nodePtr node = Alloc::allocate();
		mySTL::uninitialized_fill_n(node, 1, value);
		return node;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::fill_init(size_type n, value_type value) {
		init();
		insert(begin(), n, value);
	}

	template<class T, class Alloc>
	list<T, Alloc>::~list() {
		while (head != tail) {
			auto tmp = head++;
			deleteNode(tmp.node);
		}
		deleteNode(head.node);
	}

	template<class T, class Alloc>
	list<T, Alloc>::list() {
		init();
	}

	template<class T, class Alloc>
	list<T, Alloc>::list(size_type n) {
		fill_init(n);
	}

	template<class T, class Alloc>
	list<T, Alloc>::list(size_type n, const value_type& val) {
		fill_init(n, val);
	}

	template<class T, class Alloc>
	list<T, Alloc>::list(const list& other) {
		init();
		insert(begin(), other.begin(), other.end());
	}

	template<class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list& other) {
		if (*this == other) return *this;
		auto it1 = head;
		auto it2 = other.head;
		while (it1 != end() && it2 != other.end()) {
			*it1 = *it2;
			it1++;
			it2++;
		}
		if (it1 != end()) {
			erase(it1, end());
		}
		else {
			insert(it1, it2, other.end());
		}
		return *this;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	list<T, Alloc>::list(InputIterator first, InputIterator last) {
		init();
		insert(begin(), first, last);
	}


	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, const value_type& value) {
		if (position == begin()) {
			nodePtr node = newNode(value);
			head.node->prev = node;
			node->next = head.node;
			head.node = node;
			return begin();
		}
		nodePtr node = newNode(value);
		position.node->prev->next = node;
		node->prev = position.node->prev;
		position.node->prev = node;
		node->next = position.node;
		return --position;
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, size_type count, const value_type& value) {
		while (count > 0) {
			count--;
			insert(position, value);
			--position;
		}
		return position;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		if (first == last) return position;
		return insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	template<class Interger>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert_aux(iterator position, size_type count, const Interger value, std::true_type) {
		return insert(position, count, value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		while (last-- != first) {
			insert(position, *last);
			position--;
		}
		return position;
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position) {
		if (position == end()) return end();
		if (position == begin()) {
			pop_front();
			return begin();
		}
		auto tmp = position;
		position++;
		position.node->prev = tmp.node->prev;
		tmp.node->prev->next = position.node;
		deleteNode(tmp.node);
		return position;
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last) {
		while (first != last) {
			first = erase(first);
		}
		return last;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::push_front(const value_type& val) {
		insert(begin(), val);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::push_back(const value_type& val) {
		insert(end(), val);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::pop_front() {
		auto tmp = head;
		head++;
		head.node->prev = nullptr;
		deleteNode(tmp.node);
	}
	template<class T, class Alloc>
	void list<T, Alloc>::pop_back() {
		auto tmp = tail;
		tail--;
		tail.node->next = nullptr;
		deleteNode(tmp.node);
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::size_type list<T, Alloc>::size() const {
		size_type distance = 0;
		for (auto p = head; p != tail; p++) {
			distance++;
		}
		return distance;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::clear() {
		erase(begin(), end());
	}

	template<class T, class Alloc>
	void list<T, Alloc>::swap(list& other) {
		auto tmp = head.node;
		head.node = other.head.node;
		other.head.node = tmp;
		tmp = tail.node;
		tail.node = other.tail.node;
		other.tail.node = tmp;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& other) {
		splice(position, other, other.begin(), other.end());
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& other, iterator it) {
		if (it == other.end()) return;
		auto tmp = it;
		splice(position, other, it, ++tmp);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& other, iterator first, iterator last) {
		if (first.node == last.node) return;
		nodePtr endNode = last.node->prev;
		if (first == other.begin()) {
			other.head.node = last.node;
			other.head.node->prev = nullptr;
		}
		else {
			first.node->prev->next = last.node;
			last.node->prev = first.node->prev;
		}
		if (position == head) {
			head.node->prev = endNode;
			endNode->next = head.node;
			head.node = first.node;
			first.node->prev = nullptr;
		}
		else {
			position.node->prev->next = first.node;
			first.node->prev = position.node->prev;
			position.node->prev = endNode;
			endNode->next = position.node;
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::sort() {
		sort(mySTL::less<T>());
	}
	
	template<class T, class Alloc>
	template<class Compare>
	void list<T, Alloc>::sort(Compare comp) {
		if (empty() || head.node->next == tail.node) return;
		list<T, Alloc> carry;
		list<T, Alloc> count[64];
		int lastIdx = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < lastIdx && !count[i].empty()) {
				count[i].merge(carry, comp);
				carry.swap(count[i++]);
			}
			count[i].swap(carry);
			if (i == lastIdx) lastIdx++;
		}
		for (int i = 1; i < lastIdx; i++) {
			count[i].merge(count[i - 1], comp);
		}
		swap(count[lastIdx-1]);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::merge(list& other) {
		merge(other, mySTL::less<T>());
	}

	template<class T, class Alloc>
	template<class Compare>
	void list<T, Alloc>::merge(list& other, Compare comp) {
		auto it1 = begin();
		auto it2 = other.begin();
		while (it1 != end() && it2 != other.end()) {
			if (comp(*it2, *it1))
			{
				auto tmp = it2++;
				splice(it1, other, tmp);
			}
			else {
				it1++;
			}
		}
		if (it2 != end()) {
			splice(end(), other, it2, other.end());
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::reverse() {
		auto it = begin();
		while (it != tail) {
			auto tmp = it;
			it++;
			tmp.node->next = tmp.node->prev;
			tmp.node->prev = it.node;
		}
		auto tmpNode = head.node;
		tmpNode->next = tail.node;
		head.node = tail.node->prev;
		head.node->prev = nullptr;
		tail.node->prev = tmpNode;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		if (empty() || head.node->next == tail.node) return;
		auto it2 = begin();
		auto it1 = it2++;
		while (it1 != end() && it2 != end()) {
			if (*it1 == *it2) {
				it1.node->next = it2.node->next;
				it2.node->next->prev = it1.node;
				auto tmp = it2++;
				deleteNode(tmp.node);
			}
			else {
				it1++;
				it2++;
			}
		}
	}
	
	template<class T, class Alloc>
	template <class BinaryPredicate>
	void list<T, Alloc>::unique(BinaryPredicate binary_pred) {
		if (empty() || head.node->next == tail.node) return;
		auto it2 = begin();
		auto it1 = it2++;
		while (it1 != end() && it2 != end()) {
			if (binary_pred(*it1, *it2)) {
				it1.node->next = it2.node->next;
				it2.node->next->prev = it1.node;
				auto tmp = it2++;
				deleteNode(tmp.node);
			}
			else {
				it1++;
				it2++;
			}
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		if (empty()) return;
		auto it = begin();
		while (it != end()) {
			if (*it == value) {
				it = erase(it);
			}
			else {
				it++;
			}
		}
	}

	template<class T, class Alloc>
	template< class UnaryPredicate >
	void list<T, Alloc>::remove_if(UnaryPredicate p) {
		if (empty()) return;
		auto it = begin();
		while (it != end()) {
			if (p(*it)) {
				it = erase(it);
			}
			else {
				it++;
			}
		}
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::reference list<T, Alloc>::back() {
		auto tmp = tail;
		tmp--;
		return *tmp;
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::const_reference list<T, Alloc>::back() const {
		auto tmp = tail;
		tmp--;
		return *tmp;
	}

	template<class T, class Alloc>
	bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		if (lhs.size() != rhs.size()) return false;
		auto first1 = lhs.begin();
		auto first2 = rhs.begin();
		while (first1 != lhs.end()) {
			if (*first1 != *first2)
				return false;
			first1++;
			first2++;
		}
		return true;
	}

	template<class T, class Alloc>
	bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}
}

#endif // !_LIST_H_

