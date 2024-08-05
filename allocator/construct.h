#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
#pragma once

namespace mySTL {
	template<class T1, class T2>
	inline void construct(T1* ptr, T2& value) {
		new (ptr) T1(value);
	}

	template<class T>
	inline void destroy(T* ptr) {
		ptr->~T();
	}
}

#endif // !_CONSTRUCT_H_

