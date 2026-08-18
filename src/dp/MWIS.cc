#include "dp/MWIS.h"

#include <algorithm>

namespace guozi::dp
{

MWISResult MaxWeightIndependentSet(const std::span<const int64_t> weights)
{
	auto n = weights.size();
	if (n == 0)
	{
		return MWISResult({}, 0);
	}

	std::vector<int64_t> dp(n + 1, 0);
	dp[1] = weights[0];
	for (size_t i = 2; i <= n; i++)
	{
		dp[i] = std::max(dp[i - 1], dp[i - 2] + weights[i - 1]);
	}

	std::vector<int64_t> selectedNodes;
	for (size_t i = n; i > 0;)
	{
		if (dp[i] == dp[i - 1])
		{
			i--;
		}
		else
		{
			selectedNodes.push_back(i - 1);
			if (i == 1)
			{
				break;
			}
			i -= 2;
		}
	}
	std::reverse(selectedNodes.begin(), selectedNodes.end());

	return MWISResult(std::move(selectedNodes), dp[n]);
}

} // namespace guozi::dp
