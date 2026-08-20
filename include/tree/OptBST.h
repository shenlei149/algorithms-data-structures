#pragma once

#include "tree/BinarySearchTree.h"

#include <cstdint>
#include <span>

namespace guozi::tree
{

// OptBST short for Optimal Binary Search Tree
class OptBSTResult
{
public:
	OptBSTResult(int64_t minCost, BinarySearchTree<int32_t> &&bst)
		: minCost_(minCost)
		, bst_(std::move(bst))
	{}

	[[nodiscard]] int64_t GetMinCost() const { return minCost_; }

	[[nodiscard]] const BinarySearchTree<int32_t> &GetBST() const { return bst_; }

private:
	int64_t minCost_ { 0 };
	BinarySearchTree<int32_t> bst_;
};

// implicit that keys are 0, 1, 2, ..., n-1
OptBSTResult BuildOptimalBST(const std::span<const int64_t> weights);

} // namespace guozi::tree
