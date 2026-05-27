#pragma once

#include <vector>

namespace guozi::basic
{
class UnionFind
{
public:
	explicit UnionFind(size_t size);

	size_t Find(size_t x);

	void Union(size_t x, size_t y);

	bool Connected(size_t x, size_t y);

private:
	std::vector<size_t> parent_;
	std::vector<size_t> size_;
};

} // namespace guozi::basic
