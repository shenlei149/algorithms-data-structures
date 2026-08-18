#include <cstdint>
#include <span>
#include <vector>

namespace guozi::dp
{

class KnapsackResult
{
public:
	KnapsackResult(std::vector<int64_t> selectedItems, int64_t maxValue)
		: selectedItems_(std::move(selectedItems))
		, maxValue_(maxValue)
	{}

	[[nodiscard]] const std::vector<int64_t> &SelectedItems() const { return selectedItems_; }

	[[nodiscard]] int64_t MaxValue() const { return maxValue_; }

private:
	std::vector<int64_t> selectedItems_;
	int64_t maxValue_;
};

KnapsackResult Knapsack(const std::span<const int64_t> sizes, const std::span<const int64_t> values, int64_t capacity);

} // namespace guozi::dp
