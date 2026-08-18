#include "dp/MWIS.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

namespace
{

TEST(MWISTest, EmptyInputReturnsEmptySelection)
{
	const std::vector<int64_t> weights{};
	auto result = guozi::dp::MaxWeightIndependentSet(weights);

	EXPECT_TRUE(result.SelectedNodes().empty());
	EXPECT_EQ(0, result.MaxWeight());
}

TEST(MWISTest, AllZeroWeightsReturnEmptySelection)
{
	const std::vector<int64_t> weights{ 0, 0, 0, 0 };
	auto result = guozi::dp::MaxWeightIndependentSet(weights);

	EXPECT_TRUE(result.SelectedNodes().empty());
	EXPECT_EQ(0, result.MaxWeight());
}

TEST(MWISTest, SingleNodeReturnsThatNode)
{
	const std::vector<int64_t> weights{ 7 };
	auto result = guozi::dp::MaxWeightIndependentSet(weights);

	EXPECT_EQ((std::vector<int64_t>{ 0 }), result.SelectedNodes());
	EXPECT_EQ(7, result.MaxWeight());
}

TEST(MWISTest, ProvidedExampleUsesNonAdjacentHeavyNodes)
{
	const std::vector<int64_t> weights{ 3, 2, 1, 6, 4, 5 };
	auto result = guozi::dp::MaxWeightIndependentSet(weights);

	EXPECT_EQ((std::vector<int64_t>{ 0, 3, 5 }), result.SelectedNodes());
	EXPECT_EQ(14, result.MaxWeight());
}

TEST(MWISTest, AvoidsAdjacentNodesEvenWhenLargerTotalExists)
{
	const std::vector<int64_t> weights{ 8, 1, 7, 3, 9 };
	auto result = guozi::dp::MaxWeightIndependentSet(weights);

	EXPECT_EQ((std::vector<int64_t>{ 0, 2, 4 }), result.SelectedNodes());
	EXPECT_EQ(24, result.MaxWeight());
}

} // namespace
