#include "dp/Knapsack.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

namespace
{

TEST(KnapsackTest, EmptyInputReturnsNoItemsAndZeroValue)
{
	const std::vector<int64_t> sizes {};
	const std::vector<int64_t> values {};
	auto result = guozi::dp::Knapsack(sizes, values, 6);

	EXPECT_TRUE(result.SelectedItems().empty());
	EXPECT_EQ(0, result.MaxValue());
}

TEST(KnapsackTest, ZeroOrNegativeCapacityReturnsEmptySelection)
{
	const std::vector<int64_t> sizes { 2, 3, 4 };
	const std::vector<int64_t> values { 5, 7, 9 };
	auto result = guozi::dp::Knapsack(sizes, values, 0);

	EXPECT_TRUE(result.SelectedItems().empty());
	EXPECT_EQ(0, result.MaxValue());
}

TEST(KnapsackTest, ProvidedExampleChoosesBestSubset)
{
	const std::vector<int64_t> sizes { 4, 3, 2, 3 };
	const std::vector<int64_t> values { 3, 2, 4, 4 };
	auto result = guozi::dp::Knapsack(sizes, values, 6);

	EXPECT_EQ((std::vector<int64_t> { 2, 3 }), result.SelectedItems());
	EXPECT_EQ(8, result.MaxValue());
}

TEST(KnapsackTest, BestSubsetCanUseMultipleSmallItems)
{
	const std::vector<int64_t> sizes { 2, 2, 3, 1 };
	const std::vector<int64_t> values { 5, 4, 3, 2 };
	auto result = guozi::dp::Knapsack(sizes, values, 5);

	EXPECT_EQ((std::vector<int64_t> { 0, 1, 3 }), result.SelectedItems());
	EXPECT_EQ(11, result.MaxValue());
}

TEST(KnapsackTest, IgnoresOverCapacityItems)
{
	const std::vector<int64_t> sizes { 3, 4, 5 };
	const std::vector<int64_t> values { 10, 5, 8 };
	auto result = guozi::dp::Knapsack(sizes, values, 8);

	EXPECT_EQ((std::vector<int64_t> { 0, 2 }), result.SelectedItems());
	EXPECT_EQ(18, result.MaxValue());
}

TEST(KnapsackTest, PrefersHigherValueDensityWithoutExceedingCapacity)
{
	const std::vector<int64_t> sizes { 2, 3, 4 };
	const std::vector<int64_t> values { 7, 4, 6 };
	auto result = guozi::dp::Knapsack(sizes, values, 5);

	EXPECT_EQ((std::vector<int64_t> { 0, 1 }), result.SelectedItems());
	EXPECT_EQ(11, result.MaxValue());
}

TEST(KnapsackTest, EqualValueTiesStillReturnValidSubset)
{
	const std::vector<int64_t> sizes { 3, 3, 3 };
	const std::vector<int64_t> values { 1, 2, 3 };
	auto result = guozi::dp::Knapsack(sizes, values, 6);

	EXPECT_EQ((std::vector<int64_t> { 1, 2 }), result.SelectedItems());
	EXPECT_EQ(5, result.MaxValue());
}

TEST(KnapsackTest, SingleItemCanBeOptimalAtCapacityLimit)
{
	const std::vector<int64_t> sizes { 2, 2, 2 };
	const std::vector<int64_t> values { 1, 2, 3 };
	auto result = guozi::dp::Knapsack(sizes, values, 3);

	EXPECT_EQ((std::vector<int64_t> { 2 }), result.SelectedItems());
	EXPECT_EQ(3, result.MaxValue());
}

} // namespace
