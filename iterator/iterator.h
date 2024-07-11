#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#pragma once
namespace mySTL {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag: public input_iterator_tag {};
	struct bidrectional_iterator_tag: public forward_iterator_tag {};
	struct random_access_iterator_tag: public bidrectional_iterator_tag {};

	//distance:迭代器之间的距离
	template<class T, class Distance> struct input_iterator
	{
		typedef input_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T, class Distance> struct forward_iterator
	{
		typedef forward_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T, class Distance> struct bidrectional_iterator
	{
		typedef bidrectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T, class Distance> struct random_access_iterator
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class iterator> struct iterator_traits {
		typedef typename iterator::iterator_category iterator_category;
		typedef typename iterator::value_type value_type;
		typedef typename iterator::difference_type difference_type;
		typedef typename iterator::pointer pointer;
		typedef typename iterator::reference reference;
	};

	template<class T> struct iterator_traits<T*> {
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T>struct iterator_traits<const T*> {
		typedef  random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
}


#endif // !_ITERATOR_H

