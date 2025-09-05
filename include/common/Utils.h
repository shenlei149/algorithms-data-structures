#include <iterator>

namespace guozi::utils
{

template<std::random_access_iterator Iterator,
		 typename Greater = std::greater<typename std::iterator_traits<Iterator>::value_type>>
bool
IsSorted(Iterator begin, Iterator end, Greater greater = {})
{
	for (Iterator i = begin; i < end; i++)
	{
		if (i + 1 == end)
		{
			break;
		}

		if (greater(*i, *(i + 1)))
		{
			return false;
		}
	}

	return true;
}

} // namespace guozi::utils
