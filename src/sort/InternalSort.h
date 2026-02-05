#pragma once

#include <iterator>
#include <vector>

#include "basic/Heap.h"

namespace guozi::sort
{

namespace
{

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
size_t InternalMergeAndCountInversion(Iterator begin,
									  Iterator mid,
									  Iterator end,
									  std::vector<typename std::iterator_traits<Iterator>::value_type> &aux,
									  Less less)
{
	size_t count = 0;
	auto left = begin;
	auto right = mid;
	auto index = 0; // index of aux
	while (true)
	{
		if (left == mid) // left is exhausted
		{
			break;
		}
		else if (right == end) // right is exhausted
		{
			std::copy(left, mid, end - std::distance(left, mid));
			break;
		}
		else if (less(*right, *left))
		{
			aux[index] = *right;
			index++;
			right++;

			count += std::distance(left, mid);
		}
		else
		{
			aux[index] = *left;
			index++;
			left++;
		}
	}

	std::copy(aux.begin(), aux.begin() + index, begin);

	return count;
}

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
size_t InternalCountInversion(Iterator begin,
							  Iterator end,
							  std::vector<typename std::iterator_traits<Iterator>::value_type> &aux,
							  Less less)
{
	if (begin >= end || begin + 1 == end)
	{
		return 0;
	}

	Iterator mid = begin + (end - begin) / 2;
	auto left = InternalCountInversion(begin, mid, aux, less);
	auto right = InternalCountInversion(mid, end, aux, less);
	auto split = InternalMergeAndCountInversion(begin, mid, end, aux, less);

	return left + right + split;
}
} // namespace

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
size_t CountInversion(Iterator begin, Iterator end, Less less = {})
{
	if (begin >= end || begin + 1 == end)
	{
		return 0;
	}

	using ValueType = typename std::iterator_traits<Iterator>::value_type;
	std::vector<ValueType> aux;
	aux.resize(std::distance(begin, end));

	return InternalCountInversion(begin, end, aux, less);
}

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void SelectionSort(Iterator begin, Iterator end, Less less = {})
{
	for (Iterator i = begin; i < end; i++)
	{
		Iterator min = i;
		for (Iterator j = i + 1; j < end; j++)
		{
			if (less(*j, *min))
			{
				min = j;
			}
		}

		if (i != min)
		{
			std::swap(*i, *min);
		}
	}
}

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void InsertionSort(Iterator begin, Iterator end, Less less = {})
{
	for (Iterator i = begin + 1; i < end; i++)
	{
		for (Iterator j = i; j > begin; j--)
		{
			if (less(*j, *(j - 1)))
			{
				std::swap(*j, *(j - 1));
			}
		}
	}
}

namespace
{
template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void InternalMerge(Iterator begin,
				   Iterator mid,
				   Iterator end,
				   std::vector<typename std::iterator_traits<Iterator>::value_type> &aux,
				   Less less)
{
	auto left = begin;
	auto right = mid;
	auto index = 0; // index of aux
	while (true)
	{
		if (left == mid) // left is exhausted
		{
			break;
		}
		else if (right == end) // right is exhausted
		{
			std::copy(left, mid, end - std::distance(left, mid));
			break;
		}
		else if (less(*right, *left))
		{
			aux[index] = *right;
			index++;
			right++;
		}
		else
		{
			aux[index] = *left;
			index++;
			left++;
		}
	}

	std::copy(aux.begin(), aux.begin() + index, begin);
}

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void InternalMergeSort(Iterator begin,
					   Iterator end,
					   std::vector<typename std::iterator_traits<Iterator>::value_type> &aux,
					   Less less)
{
	if (begin >= end || begin + 1 == end)
	{
		return;
	}

	Iterator mid = begin + (end - begin) / 2;
	InternalMergeSort(begin, mid, aux, less);
	InternalMergeSort(mid, end, aux, less);
	InternalMerge(begin, mid, end, aux, less);
}
} // namespace

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void MergeSort(Iterator begin, Iterator end, Less less = {})
{
	if (begin >= end || begin + 1 == end)
	{
		return;
	}

	using ValueType = typename std::iterator_traits<Iterator>::value_type;
	std::vector<ValueType> aux;
	aux.resize(std::distance(begin, end));

	InternalMergeSort(begin, end, aux, less);
}

namespace
{
template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
Iterator QuickSortPartition(Iterator begin, Iterator end, Less less)
{
	Iterator i = begin + 1;
	Iterator p = begin;
	for (Iterator j = i; j < end; j++)
	{
		if (less(*j, *p))
		{
			std::swap(*i, *j);
			i++;
		}
	}

	std::swap(*p, *(i - 1));

	return i - 1;
}
} // namespace

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void QuickSort(Iterator begin, Iterator end, Less less = {})
{
	if (begin >= end || begin + 1 == end)
	{
		return;
	}

	auto p = QuickSortPartition(begin, end, less);
	QuickSort(begin, p, less);
	QuickSort(p + 1, end, less);
}

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void HeapSort(Iterator begin, Iterator end, Less less = {})
{
	using ValueType = typename std::iterator_traits<Iterator>::value_type;
	guozi::basic::Heap<ValueType, Less> heap(begin, end, less);

	for (Iterator it = begin; it < end; it++)
	{
		*it = heap.Top();
		heap.Pop();
	}
}

} // namespace guozi::sort
