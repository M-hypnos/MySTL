#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

#pragma once
namespace mySTL {
	struct _true_type {};
	struct _false_type {};

	template <class T>
	struct type_traits {
		typedef _false_type is_POD_type;
	};

	template<> struct type_traits<char> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<bool> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<short> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<unsigned short> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<int> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<unsigned int> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<long> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<unsigned long> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<long long> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<unsigned long long> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<float> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<double> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<long double> {
		typedef _true_type is_POD_type;
	};

	template<class T> struct type_traits<T*> {
		typedef _true_type is_POD_type;
	};

	template<class T> struct type_traits<const T*> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<char*> {
		typedef _true_type is_POD_type;
	};

	template<> struct type_traits<const char*> {
		typedef _true_type is_POD_type;
	};
}


#endif // !_TYPE_TRAITS_

