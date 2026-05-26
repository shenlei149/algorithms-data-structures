#include "basic/UnionFind.h"

#include <numeric>

namespace guozi::basic
{

UnionFind::UnionFind(size_t size)
	: parent_(size)
	, size_(size, 1)
{
	std::iota(parent_.begin(), parent_.end(), 0);
}

size_t UnionFind::Find(size_t x)
{
	if (parent_[x] != x)
	{
		// path halving: make every other node in the path point to its grandparent
		parent_[x] = parent_[parent_[x]];
		x = parent_[x];
	}

	return x;
}

void UnionFind::Union(size_t x, size_t y)
{
	size_t rootX = Find(x);
	size_t rootY = Find(y);

	if (rootX == rootY)
	{
		return; // already in the same set
	}

	if (size_[rootX] < size_[rootY])
	{
		std::swap(rootX, rootY);
	}

	// attach the smaller tree to the larger one
	// since rootX is guaranteed to be the larger tree, we attach rootY to rootX
	parent_[rootY] = rootX;
	size_[rootX] += size_[rootY];
}

bool UnionFind::Connected(size_t x, size_t y) { return Find(x) == Find(y); }

} // namespace guozi::basic
