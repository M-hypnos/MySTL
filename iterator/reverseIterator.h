#ifndef _REVERSE_ITERATOR_H_
#define _REVERSE_ITERATOR_H_
#pragma once

#include "iterator.h"

namespace mySTL {
	template<class iterator>
	class reverse_iterator {
		protected:
			iterator current;
		public:
			typedef typename iterator_traits<iterator>::iterator_category iterator_category;
			typedef typename iterator_traits<iterator>::value_type value_type;
			typedef typename iterator_traits<iterator>::difference_type difference_type;
			typedef typename iterator_traits<iterator>::pointer pointer;
			typedef typename iterator_traits<iterator>::reference reference;

			typedef iterator iterator_type;
		public:
			explicit reverse_iterator(iterator_type x):current(x){}
			template<class iter>
			reverse_iterator(const reverse_iterator<iter>& x):current(x.current){}


			iterator_type base() const { return current; }
			reference operator*() const { iterator it = current; return *--it; }
			pointer operator->() const { return --current; }

			reverse_iterator& operator++() { current--; return *this; }
			reverse_iterator& operator--() { current++; return *this; }
			reverse_iterator operator+(difference_type n) const { return self(current - n); }
			reverse_iterator operator-(difference_type n) const { return self(current + n); }

			template <class iterator>
			friend bool operator== (const reverse_iterator<iterator>& lhs,
				const reverse_iterator<iterator>& rhs);
			template <class iterator>
			friend bool operator!= (const reverse_iterator<iterator>& lhs,
				const reverse_iterator<iterator>& rhs);
	};

	template<class iterator>
	bool operator== (const reverse_iterator<iterator>& lhs, const reverse_iterator<iterator>& rhs) {
		return lhs.current == rhs.current;
	}
	template<class iterator>
	bool operator!= (const reverse_iterator<iterator>& lhs, const reverse_iterator<iterator>& rhs) {
		return !(lhs == rhs);
	}
}

#endif // !_REVERSE_ITERATOR_H_

