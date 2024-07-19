#ifndef _HEAP_H_
#define _HEAP_H_
#pragma once
#include "../functors.h"
#include "../iterator/iterator.h"

namespace mySTL {
	template<class RandomAccessIterator, class Compare>
	void down(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator head, Compare comp) {
		if (head == last) return;
		if (first == last) return;
		auto len = last - head;
		auto topIdx = first - head;
		auto leftChildIdx = 2 * topIdx + 1;
		while (leftChildIdx < len) {
			auto changeIdx = topIdx;
			if (comp(*(head + leftChildIdx), *(head + topIdx))) {
				changeIdx = leftChildIdx;
			}
			if (leftChildIdx + 1 < len && comp(*(head + leftChildIdx + 1), *(head + changeIdx))) {
				changeIdx = leftChildIdx + 1;
			}
			if (changeIdx == topIdx) break;
			auto tmp = *(head + changeIdx);
			*(head + changeIdx) = *(head + topIdx);
			*(head + topIdx) = tmp;
			topIdx = changeIdx;
			leftChildIdx = 2 * topIdx + 1;
		}
	}

	template<class RandomAccessIterator, class Compare>
	void up(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator head, Compare comp) {
		if (head == last) return;
		if (first == last) return;
		auto bottomIdx = last - head;
		auto parentIdx = (bottomIdx - 1) / 2;
		auto initIdx = first - head;
		while (parentIdx >= initIdx) {
			if (comp(*(head + bottomIdx), *(head + parentIdx))) {
				auto tmp = *(head + parentIdx);
				*(head + parentIdx) = *(head + bottomIdx);
				*(head + bottomIdx) = tmp;
				bottomIdx = parentIdx;
				parentIdx = (bottomIdx - 1) / 2;
			}
			else {
				break;
			}
		}
	}

	template<class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		typedef typename mySTL::iterator_traits<RandomAccessIterator>::value_type value_type;
		pop_heap(first, last, mySTL::greater<value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		auto tmp = *(last - 1);
		*(last - 1) = *first;
		*first = tmp;
		down(first, last - 1, first, comp);
	}

	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		typedef typename mySTL::iterator_traits<RandomAccessIterator>::value_type value_type;
		push_heap(first, last, mySTL::greater<value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		up(first, last - 1, first, comp);
	}

	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		typedef typename mySTL::iterator_traits<RandomAccessIterator>::value_type value_type;
		sort_heap(first, last, mySTL::greater<value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		while (last - first > 0) {
			pop_heap(first, last--, comp);
		}
	}

	template<class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
		typedef typename mySTL::iterator_traits<RandomAccessIterator>::value_type value_type;
		make_heap(first, last, mySTL::greater<value_type>());
	}

	template<class RandomAccessIterator, class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		auto n = last - first;
		if (n < 2) return;
		int lastRootIdx = n / 2 - 1;
		for (auto i = lastRootIdx; i >= 0; i--) {
			down(first + i, last, first, comp);
		}
	}
}

#endif // !_HEAP_H_

