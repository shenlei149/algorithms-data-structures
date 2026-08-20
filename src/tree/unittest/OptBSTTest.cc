#include "tree/OptBST.h"

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

namespace
{

std::vector<int32_t> PreOrderValues(const guozi::tree::BinarySearchTree<int32_t> &tree)
{
	std::vector<int32_t> values;
	tree.ForEachPreOrder([&values](int32_t value) { values.push_back(value); });
	return values;
}

std::vector<int32_t> InOrderValues(const guozi::tree::BinarySearchTree<int32_t> &tree)
{
	std::vector<int32_t> values;
	tree.ForEachInOrder([&values](int32_t value) { values.push_back(value); });
	return values;
}

void ExpectOptimalBST(const std::vector<int64_t> &weights,
					  int64_t expectedCost,
					  const std::vector<int32_t> &expectedPreOrder)
{
	const auto result = guozi::tree::BuildOptimalBST(weights);
	const auto &tree = result.GetBST();

	EXPECT_EQ(expectedCost, result.GetMinCost());
	EXPECT_EQ(weights.size(), tree.Size());
	EXPECT_EQ(expectedPreOrder, PreOrderValues(tree));

	std::vector<int32_t> expectedInOrder;
	for (size_t i = 0; i < weights.size(); ++i)
	{
		expectedInOrder.push_back(static_cast<int32_t>(i));
	}
	EXPECT_EQ(expectedInOrder, InOrderValues(tree));

	for (size_t i = 0; i < weights.size(); ++i)
	{
		const int32_t key = static_cast<int32_t>(i);
		auto it = tree.Find(key);
		ASSERT_NE(tree.end(), it);
		EXPECT_EQ(key, *it);
		EXPECT_EQ(static_cast<int>(i), tree.Rank(key));
		EXPECT_EQ(key, *tree.Select(static_cast<int>(i)));
	}

	if (weights.empty())
	{
		EXPECT_EQ(tree.begin(), tree.end());
		return;
	}

	EXPECT_EQ(0, *tree.Min());
	EXPECT_EQ(static_cast<int32_t>(weights.size() - 1), *tree.Max());
	EXPECT_EQ(tree.end(), tree.Find(-1));
	EXPECT_EQ(tree.end(), tree.Find(static_cast<int32_t>(weights.size())));
}

} // namespace

TEST(OptBSTTest, HandlesEmptyWeights)
{
	const std::vector<int64_t> weights;
	const auto result = guozi::tree::BuildOptimalBST(weights);

	EXPECT_EQ(0, result.GetMinCost());
	EXPECT_EQ(0, result.GetBST().Size());
	EXPECT_EQ(result.GetBST().begin(), result.GetBST().end());
}

TEST(OptBSTTest, HandlesSingleWeight) { ExpectOptimalBST({ 7 }, 7, { 0 }); }

TEST(OptBSTTest, ChoosesRootAccordingToWeights)
{
	ExpectOptimalBST({ 1, 3 }, 5, { 1, 0 });
	ExpectOptimalBST({ 3, 1 }, 5, { 0, 1 });
}

TEST(OptBSTTest, BuildsMultiLevelOptimalTree)
{
	ExpectOptimalBST({ 1, 2, 3 }, 10, { 1, 0, 2 });
	ExpectOptimalBST({ 25, 10, 20 }, 95, { 0, 2, 1 });
}

TEST(OptBSTTest, BuildsOptimalTreeForTenKeys)
{
	ExpectOptimalBST({ 3, 7, 2, 9, 4, 8, 1, 6, 5, 10 }, 141, { 5, 3, 1, 0, 2, 4, 8, 7, 6, 9 });
}

TEST(OptBSTTest, UsesFirstRootWhenCostsTie) { ExpectOptimalBST({ 1, 1, 1 }, 5, { 1, 0, 2 }); }
