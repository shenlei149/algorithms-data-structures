#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace guozi::dp
{

// MWIS short for Maximum Weight Independent Set
class MWISResult
{
public:
	MWISResult(std::vector<int64_t> selectedNodes, int64_t maxWeight)
		: selectedNodes_(std::move(selectedNodes))
		, maxWeight_(maxWeight)
	{}

	[[nodiscard]] const std::vector<int64_t> &SelectedNodes() const { return selectedNodes_; }

	[[nodiscard]] int64_t MaxWeight() const { return maxWeight_; }

private:
	std::vector<int64_t> selectedNodes_;
	int64_t maxWeight_;
};

MWISResult MaxWeightIndependentSet(const std::span<const int64_t> weights);

} // namespace guozi::dp
