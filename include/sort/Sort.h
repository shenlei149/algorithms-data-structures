#pragma once

#include <iterator>

namespace guozi::sort
{
template<std::random_access_iterator Iterator,
		 typename Less = std::less<typename std::iterator_traits<Iterator>::value_type>>
void Sort(Iterator begin, Iterator end, Less less = {});
} // namespace guozi::sort
