#pragma once
#ifndef _MAP_H_
#define _MAP_H_
#include "../functors.h"
#include "rb_tree.h"
#include <utility>

namespace mySTL {
	template<class Key, class T, class Compare = mySTL::less<Key>, class Alloc = Allocator<mySTL::rb_tree_detail::rb_tree_node<std::pair<const Key, T>>>>
	class map {
		public:
			typedef Key key_type;
			typedef T data_type;
			typedef std::pair<const Key, T> value_type;
			typedef Compare key_compare;

			class value_compare :public binary_function<value_type, value_type, bool > {
				friend class map<Key, T, Compare, Alloc>;
				protected:
					Compare comp;
					value_compare(Compare c):comp(c){}
				public:
					bool operator()(const value_type& x, const value_type& y) const {
						return comp(x.first, y.first);
					}
			};
		private:
			typedef mySTL::rb_tree<key_type, value_type, mySTL::select1st<value_type>, key_compare, Alloc>rep_type;
			rep_type tree;
		public:
			typedef typename rep_type::pointer pointer;
			typedef typename rep_type::const_pointer const_pointer;
			typedef typename rep_type::reference reference;
			typedef typename rep_type::const_reference const_reference;
			typedef typename rep_type::size_type size_type;
			typedef typename rep_type::difference_type difference_type;
			typedef typename rep_type::iterator iterator;
			typedef typename rep_type::const_iterator const_iterator;

			map(): tree(key_compare()){}
			explicit map(const key_compare& comp): tree(comp){}
			template <class InputIterator>
			map(InputIterator first, InputIterator last) : tree(key_compare()) {
				tree.insert_unique(first, last);
			}
			map(const map& other):tree(other.tree){}
			map& operator=(const map& other) {
				if (*this == other) return *this;
				tree = other.tree;
				return *this;
			}
			key_compare key_comp() const { return key_compare(); }

			value_compare value_comp() const { return value_compare(key_compare()); }

			iterator begin() { return tree.begin(); }
			const_iterator begin() const { return tree.begin(); }
			iterator end() { return tree.end(); }
			const_iterator end() const { return tree.end(); }

			bool empty() { return tree.empty(); }
			size_type size() { return tree.size(); }
			void clear() { tree.clear(); }

			data_type& operator[](const key_type& key) {
				iterator it = tree.lower_bound(key);
				if (it == end() || key_comp()(key, (*it).first))
					it = tree.insert_unique(it, value_type(key, data_type()));
				return (*it).second;
				//return (*((insert_unique(value_type(key, data_type()))).first)).second;
			}

			std::pair<iterator, bool> insert(const value_type& value) {
				return tree.insert_unique(value);
			}

			iterator insert(iterator pos, const value_type& value) {
				return tree.insert_unique(pos, value);
			}

			template< class InputIterator>
			void insert(InputIterator first, InputIterator last) {
				tree.insert_unique(first, last);
			}

			void erase(iterator pos) {
				return tree.erase(pos);
			}

			void erase(iterator first, iterator last) {
				return tree.erase(first, last);
			}

			size_type erase(const key_type& k) {
				return tree.erase(k);
			}

			iterator lower_bound(const key_type& k) {
				return tree.lower_bound(k);
			}
			const_iterator lower_bound(const key_type& k) const {
				return tree.lower_bound(k);
			}

			iterator upper_bound(const key_type& k) {
				return tree.upper_bound(k);
			}
			const_iterator upper_bound(const key_type& k) const {
				return tree.upper_bound(k);
			}

			std::pair<iterator, iterator> equal_range(const key_type& k) {
				return tree.equal_range(k);
			}
			std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
				return tree.equal_range(k);
			}

			size_type count(const key_type& k) const {
				return tree.count(k);
			}

			iterator find(const key_type& k) {
				return tree.find(k);
			}
			const_iterator find(const key_type& k) const {
				return tree.find(k);
			}
			void swap(map& other) {
				tree.swap(other.tree);
			}

			friend bool operator==(const map& lhs, const map& rhs) { return lhs.tree == rhs.tree; }
			friend bool operator!=(const map& lhs, const map& rhs) { return !(lhs.tree == rhs.tree); }
	};
}

#endif // !_MAP_H_
