#include "dp/Knapsack.h"

#include <algorithm>

namespace guozi::dp
{

KnapsackResult Knapsack(const std::span<const int64_t> sizes, const std::span<const int64_t> values, int64_t capacity)
{
	auto n = sizes.size();
	if (n == 0 || capacity <= 0)
	{
		return KnapsackResult({}, 0);
	}

	std::vector<std::vector<int64_t>> dp(n + 1, std::vector<int64_t>(capacity + 1, 0));
	for (size_t i = 1; i <= n; i++)
	{
		for (int64_t c = 0; c <= capacity; c++)
		{
			if (sizes[i - 1] <= c)
			{
				dp[i][c] = std::max(dp[i - 1][c], dp[i - 1][c - sizes[i - 1]] + values[i - 1]);
			}
			else
			{
				dp[i][c] = dp[i - 1][c];
			}
		}
	}

	std::vector<int64_t> selectedItems;
	int64_t c = capacity;
	for (size_t i = n; i >= 1 && c >= 0;)
	{
		if (dp[i][c] != dp[i - 1][c])
		{
			selectedItems.push_back(i - 1);
			c -= sizes[i - 1];
		}

		i--;
	}
	std::reverse(selectedItems.begin(), selectedItems.end());

	return KnapsackResult(std::move(selectedItems), dp[n][capacity]);
}

} // namespace guozi::dp
