#include <iterator>

namespace guozi::utils
{

template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
bool IsSorted(Iterator begin, Iterator end, Less less = {})
{
	for (Iterator i = begin; i < end; i++)
	{
		if (i + 1 == end)
		{
			break;
		}

		if (less(*(i + 1), *i))
		{
			return false;
		}
	}

	return true;
}

} // namespace guozi::utils
