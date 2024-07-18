#ifndef _SLIST_H_
#define _SLIST_H_
#pragma once
#include "../allocator/allocator.h"
#include "../iterator/iterator.h"
#include "../allocator/uninitialized.h"
#include "../functors.h"

namespace mySTL {
	namespace slist_detail {
		template<class T>
		struct slist_node
		{
			slist_node* next;
			T data;
			slist_node(const T& val) :data(val), next(nullptr){}
		};

		template<class T>
		struct slist_iterator {
			typedef forward_iterator_tag iterator_category;
			typedef T value_type;
			typedef ptrdiff_t difference_type;
			typedef T* pointer;
			typedef T& reference;
			typedef slist_node<T>* nodePtr;

			nodePtr node;
			slist_iterator() = default;
			explicit slist_iterator(nodePtr x) :node(x) {}
			slist_iterator(const slist_iterator& x) :node(x.node) {}

			reference operator*() const { return node->data; }
			pointer operator->() const { return &(operator*()); }

			slist_iterator operator++(int) {
				slist_iterator tmp = *this;
				++(*this);
				return tmp;
			}
			slist_iterator& operator++() {
				node = node->next;
				return *this;
			}

			template<class T>
			friend bool operator== (const slist_iterator<T>& lhs, const slist_iterator<T>& rhs);
			template<class T>
			friend bool operator!= (const slist_iterator<T>& lhs, const slist_iterator<T>& rhs);
		};

		template<class T>
		bool operator==(const slist_iterator<T>& lhs, const slist_iterator<T>& rhs) {
			return lhs.node == rhs.node;
		}
		template<class T>
		bool operator!=(const slist_iterator<T>& lhs, const slist_iterator<T>& rhs) {
			return !(lhs.node == rhs.node);
		}
	}

	template<class T, class Alloc = Allocator<slist_detail::slist_node<T>>>
	class slist {
	public:
		typedef size_t size_type;
		typedef T value_type;
		typedef slist_detail::slist_iterator<T> iterator;
		typedef ptrdiff_t difference_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;

		typedef const iterator const_iterator;
		typedef slist_detail::slist_node<T>* nodePtr;
	private:
		iterator head;
	public:
		slist();
		explicit slist(size_type n);
		explicit slist(size_type n, const value_type& val);
		slist(const slist& other);
		template<class InputIterator>
		slist(InputIterator first, InputIterator last);
		slist& operator=(const slist& other);

		~slist();

		iterator before_begin() { return head; }
		const_iterator before_begin() const { return head; }
		const_iterator cbefore_begin() const { return head; }
		iterator begin() { return iterator(head.node->next); }
		const_iterator begin() const { return const_iterator(head.node->next); }
		const_iterator cbegin() const { return const_iterator(head.node->next); }
		iterator end() { return iterator(); }
		const_iterator end() const { return const_iterator(); }
		const_iterator cend() const { return const_iterator(); }

		reference front() { return head.node->next->data; }
		const_reference front() const { return head.node->next->data; }

		iterator insert_after(iterator position, const value_type& value);
		iterator insert_after(iterator position, size_type count, const value_type& value);
		template<class InputIterator>
		iterator insert_after(iterator position, InputIterator first, InputIterator last);

		iterator erase_after(iterator position);
		iterator erase_after(iterator first, iterator last);

		void push_front(const value_type& val);
		void pop_front();

		bool empty() const { return head.node->next == nullptr; }

		void swap(slist& other);
		void clear();
		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void splice_after(iterator position, slist& other);
		void splice_after(iterator position, slist& other, iterator it);
		void splice_after(iterator position, slist& other, iterator first, iterator last);

		void sort();
		template<class Compare>
		void sort(Compare comp);

		void merge(slist& other);
		template<class Compare>
		void merge(slist& other, Compare comp);

		void reverse();

		void unique();
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);

		void remove(const T& value);
		template< class UnaryPredicate >
		void remove_if(UnaryPredicate p);

		template<class T, class Alloc>
		friend bool operator==(const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs);
		template<class T, class Alloc>
		friend bool operator!=(const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs);

	private:
		nodePtr newNode(value_type value = value_type());
		void deleteNode(nodePtr ptr);
		void init();

		template<class Interger>
		iterator insert_after_aux(iterator position, size_type count, const Interger value, std::true_type);
		template<class InputIterator>
		iterator insert_after_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
	};

	template<class T, class Alloc>
	typename slist<T, Alloc>::nodePtr slist<T, Alloc>::newNode(value_type value) {
		nodePtr node = Alloc::allocate();
		mySTL::uninitialized_fill_n(node, 1, value);
		return node;
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::deleteNode(nodePtr ptr) {
		Alloc::destroy(ptr);
		Alloc::deallocate(ptr);
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::init() {
		nodePtr node = newNode();
		head.node = node;
	}


	template<class T, class Alloc>
	slist<T, Alloc>::~slist() {
		erase_after(before_begin(), end());
		deleteNode(head.node);
	}

	template<class T, class Alloc>
	slist<T, Alloc>::slist() {
		init();
	}

	template<class T, class Alloc>
	slist<T, Alloc>::slist(size_type n) {
		init();
		insert_after(before_begin(), n, value_type());
	}

	template<class T, class Alloc>
	slist<T, Alloc>::slist(size_type n, const value_type& val) {
		init();
		insert_after(before_begin(), n, val);
	}

	template<class T, class Alloc>
	slist<T, Alloc>::slist(const slist& other) {
		init();
		insert_after(before_begin(), other.begin(), other.end());
	}

	template<class T, class Alloc>
	slist<T, Alloc>& slist<T, Alloc>::operator=(const slist& other) {
		if (*this == other) return *this;
		auto it1 = before_begin();
		auto it2 = other.before_begin();
		while (it1.node->next != nullptr && it2.node->next != nullptr) {
			it1++;
			it2++;
			*it1 = *it2;
		}
		if (it1.node->next != nullptr) {
			erase_after(it1, end());
		}
		else {
			insert_after(it1, ++it2, other.end());
		}
		return *this;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	slist<T, Alloc>::slist(InputIterator first, InputIterator last) {
		init();
		insert_after(before_begin(), first, last);
	}

	template<class T, class Alloc>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::erase_after(iterator position) {
		if (position == end()) return end();
		if (position.node->next == nullptr) return end();
		auto node = position.node->next->next;
		deleteNode(position.node->next);
		position.node->next = node;
		return iterator(node);
	}

	template<class T, class Alloc>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::erase_after(iterator first, iterator last) {
		if (first == last) return last;
		if (first.node->next == last.node) return last;
		while (first.node->next != last.node) {
			erase_after(first);
		}
		return last;
	}

	template<class T, class Alloc>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::insert_after(iterator position, const value_type& value) {
		nodePtr node = newNode(value);
		auto nextNode = position.node->next;
		position.node->next = node;
		node->next = nextNode;
		return ++position;
	}

	template<class T, class Alloc>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::insert_after(iterator position, size_type count, const value_type& value) {
		auto pos = position;
		while (count > 0) {
			count--;
			pos = insert_after(pos, value);
		}
		return pos;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::insert_after(iterator position, InputIterator first, InputIterator last) {
		if (first == last) return position;
		return insert_after_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	template<class Interger>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::insert_after_aux(iterator position, size_type count, const Interger value, std::true_type) {
		return insert_after(position, count, value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	typename slist<T, Alloc>::iterator slist<T, Alloc>::insert_after_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		auto pos = position;
		auto it = first;
		while (it != last) {
			pos = insert_after(pos, *it++);
		}
		return pos;
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::push_front(const value_type& val) {
		insert_after(before_begin(), val);
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::pop_front() {
		erase_after(before_begin());
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::swap(slist& other) {
		auto tmp = head.node;
		head.node = other.head.node;
		other.head.node = tmp;
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::clear() {
		erase_after(before_begin(), end());
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::resize(size_type count) {
		resize(count, value_type());
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::resize(size_type count, const value_type& value) {
		auto it = before_begin();
		while (it.node->next != nullptr && count > 0) {
			it++;
			count--;
		}
		if (count == 0) return;
		if (count > 0) {
			insert_after(it, count, value);
		}
		else {
			erase_after(it, end());
		}
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::splice_after(iterator position, slist& other) {
		splice_after(position, other, other.before_begin(), other.end());
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::splice_after(iterator position, slist& other, iterator it) {
		if (it == other.end()) return;
		auto insertNode = it.node->next;
		it.node->next = insertNode->next;
		insertNode->next = position.node->next;
		position.node->next = insertNode;
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::splice_after(iterator position, slist& other, iterator first, iterator last) {
		if (first.node == last.node) return;
		auto insertNode = first.node->next;
		first.node->next = last.node;
		auto nextNode = position.node->next;
		position.node->next = insertNode;
		while (insertNode->next != last.node) {
			insertNode = insertNode->next;
		}
		insertNode->next = nextNode;
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::merge(slist& other) {
		merge(other, mySTL::less<T>());
	}

	template<class T, class Alloc>
	template<class Compare>
	void slist<T, Alloc>::merge(slist& other, Compare comp) {
		auto it1 = before_begin();
		auto it2 = other.before_begin();
		while (it1.node->next != nullptr && it2.node->next != nullptr) {
			if (comp(it2.node->next->data, it1.node->next->data)) {
				splice_after(it1, other, it2);
			}
			it1++;
		}
		if (it2.node->next != nullptr) {
			splice_after(it1, other, it2, other.end());
		}
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::sort() {
		sort(mySTL::less<T>());
	}

	template<class T, class Alloc>
	template<class Compare>
	void slist<T, Alloc>::sort(Compare comp) {
		if (empty() || head.node->next->next == nullptr) return;
		slist<T, Alloc> carry;
		slist<T, Alloc> count[64];
		int lastIdx = 0;
		while (!empty()) {
			carry.splice_after(carry.before_begin(), *this, before_begin());
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
		swap(count[lastIdx - 1]);
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::reverse() {
		auto headNode = head.node;
		nodePtr leftNode = nullptr;
		auto rightNode = headNode->next;
		if (rightNode == nullptr) return;
		while (rightNode != nullptr) {
			auto nextNode = rightNode->next;
			rightNode->next = leftNode;
			leftNode = rightNode;
			rightNode = nextNode;
		}
		headNode->next = leftNode;
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::unique() {
		if (empty() || head.node->next->next == nullptr) return;
		auto it1 = begin();
		auto it2 = iterator(head.node->next->next);
		while (it2 != end()) {
			if (*it1 == *it2) {
				it1.node->next = it2.node->next;
				auto tmpNode = it2.node;
				it2++;
				deleteNode(tmpNode);
			}
			else {
				it1++;
				it2++;
			}
		}
	}

	template<class T, class Alloc>
	template <class BinaryPredicate>
	void slist<T, Alloc>::unique(BinaryPredicate binary_pred) {
		if (empty() || head.node->next->next == nullptr) return;
		auto it1 = begin();
		auto it2 = iterator(head.node->next->next);
		while (it2 != end()) {
			if (binary_pred(*it1, *it2)) {
				it1.node->next = it2.node->next;
				auto tmpNode = it2.node;
				it2++;
				deleteNode(tmpNode);
			}
			else {
				it1++;
				it2++;
			}
		}
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::remove(const T& value) {
		if (empty()) return;
		auto it = before_begin();
		while (it.node->next != nullptr) {
			if (it.node->next->data == value) {
				erase_after(it);
			}
			else {
				it++;
			}
		}
	}

	template<class T, class Alloc>
	template< class UnaryPredicate >
	void slist<T, Alloc>::remove_if(UnaryPredicate p) {
		if (empty()) return;
		auto it = before_begin();
		while (it.node->next != nullptr) {
			if (p(it.node->next->data)) {
				erase_after(it);
			}
			else {
				it++;
			}
		}
	}

	template<class T, class Alloc>
	bool operator==(const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs) {
		auto first1 = lhs.begin();
		auto first2 = rhs.begin();
		while (first1 != lhs.end() && first2 != rhs.end()) {
			if (*first1 != *first2)
				return false;
			first1++;
			first2++;
		}
		return first1 == lhs.end() && first2 == rhs.end();
	}

	template<class T, class Alloc>
	bool operator!=(const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}
}

#endif // !_SLIST_H_

