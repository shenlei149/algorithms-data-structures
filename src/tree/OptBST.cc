#include "tree/OptBST.h"

#include <limits>
#include <vector>

namespace guozi::tree
{

namespace
{

void
CollectPreOrder(int32_t i, int32_t j, const std::vector<std::vector<int32_t>> &root, std::vector<int32_t> &preOrder)
{
	if (i > j)
	{
		return;
	}

	int32_t r = root[i][j];

	// push r - 1 because the keys are 0, 1, 2, ..., n-1
	preOrder.push_back(r - 1);
	CollectPreOrder(i, r - 1, root, preOrder);
	CollectPreOrder(r + 1, j, root, preOrder);
}

} // namespace

OptBSTResult BuildOptimalBST(const std::span<const int64_t> weights)
{
	auto n = weights.size();
	if (n == 0)
	{
		return OptBSTResult(0, BinarySearchTree<int32_t>());
	}

	// 1-based indexing
	std::vector<std::vector<int64_t>> cost(n + 2, std::vector<int64_t>(n + 1, 0));
	std::vector<std::vector<int32_t>> root(n + 2, std::vector<int32_t>(n + 1, -1));

	std::vector<int64_t> prefixSum(n + 1, 0);
	for (size_t i = 1; i <= n; ++i)
	{
		prefixSum[i] = prefixSum[i - 1] + weights[i - 1];
	}

	for (size_t s = 0; s < n; s++)
	{
		for (size_t i = 1; i <= n - s; i++)
		{
			size_t j = i + s;
			cost[i][j] = std::numeric_limits<int64_t>::max();

			int64_t totalWeight = prefixSum[j] - prefixSum[i - 1];

			for (size_t r = i; r <= j; ++r)
			{
				int64_t c = cost[i][r - 1] + cost[r + 1][j] + totalWeight;

				if (c < cost[i][j])
				{
					cost[i][j] = c;
					root[i][j] = static_cast<int32_t>(r);
				}
			}
		}
	}

	std::vector<int32_t> preOrder;
	preOrder.reserve(n);
	CollectPreOrder(1, static_cast<int32_t>(n), root, preOrder);
	BinarySearchTree<int32_t> bst = BinarySearchTree<int32_t>::FromPreOrder(preOrder);

	return OptBSTResult(cost[1][n], std::move(bst));
}

} // namespace guozi::tree
