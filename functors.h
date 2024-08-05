#ifndef _FUNCTORS_H_
#define _FUNCTORS_H_
#pragma once

namespace mySTL {
	template<class Arg1, class Arg2, class Result>
	struct binary_function
	{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	template<class T>
	struct less:public binary_function<T,T,bool>
	{
		bool operator()(const T& x, const T& y) const {
			return x < y;
		}
	};
	
	template<class T>
	struct greater:public binary_function<T,T,bool>
	{
		bool operator()(const T& x, const T& y) const {
			return x > y;
		}
	};

	template<class Arg, class Result>
	struct unary_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

	template<class T>
	struct identify :public unary_function<T, T>
	{
		const T& operator()(const T& x) const {
			return x;
		}
	};
}

#endif // !_FUNCTORS_H_

