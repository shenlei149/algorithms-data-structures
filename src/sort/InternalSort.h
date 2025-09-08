#include <iterator>
#include <vector>

namespace guozi::sort
{

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void
SelectionSort(Iterator begin, Iterator end, Less less = {})
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
void
InsertionSort(Iterator begin, Iterator end, Less less = {})
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
void
InternalMerge(Iterator begin,
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
void
InternalMergeSort(Iterator begin,
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
void
MergeSort(Iterator begin, Iterator end, Less less = {})
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

} // namespace guozi::sort
