#include "tree/BinarySearchTree.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include <gtest/gtest.h>

namespace
{
std::vector<int> InOrderValues(const guozi::tree::BinarySearchTree<int> &tree)
{
	std::vector<int> values;
	tree.ForEach([&values](int value) { values.push_back(value); });
	return values;
}

std::vector<int> BuildShuffledInsertionOrder(int left, int right, unsigned int seed)
{
	if (left > right)
	{
		return {};
	}

	std::vector<int> order(static_cast<size_t>(right - left + 1));
	std::iota(order.begin(), order.end(), left);

	std::mt19937 generator(seed);
	std::shuffle(order.begin(), order.end(), generator);
	return order;
}

void EraseValue(std::vector<int> &values, int target)
{
	auto it = std::find(values.begin(), values.end(), target);
	if (it != values.end())
	{
		values.erase(it);
	}
}

void ExpectTreeMatches(const guozi::tree::BinarySearchTree<int> &tree, const std::vector<int> &expected)
{
	EXPECT_EQ(expected.size(), tree.Size());
	EXPECT_EQ(expected, InOrderValues(tree));

	std::vector<int> forward;
	for (auto it = tree.begin(); it != tree.end(); ++it)
	{
		forward.push_back(*it);
	}
	EXPECT_EQ(expected, forward);

	if (expected.empty())
	{
		EXPECT_EQ(tree.begin(), tree.end());
		EXPECT_EQ(tree.Min(), tree.end());
		EXPECT_EQ(tree.Max(), tree.end());
		EXPECT_EQ(tree.Predecessor(tree.end()), tree.end());
		EXPECT_EQ(tree.Successor(tree.end()), tree.end());
		return;
	}

	EXPECT_EQ(expected.front(), *tree.Min());
	EXPECT_EQ(expected.back(), *tree.Max());
	EXPECT_EQ(*tree.Max(), *tree.Predecessor(tree.end()));
	EXPECT_EQ(tree.end(), tree.Successor(tree.end()));

	std::vector<int> backward;
	for (auto it = tree.Max(); it != tree.end(); it = tree.Predecessor(it))
	{
		backward.push_back(*it);
		if (it == tree.Min())
		{
			break;
		}
	}

	std::vector<int> expectedBackward(expected.rbegin(), expected.rend());
	EXPECT_EQ(expectedBackward, backward);
}
} // namespace

TEST(BinarySearchTreeTest, BoundaryEmptyTree)
{
	guozi::tree::BinarySearchTree<int> tree;

	EXPECT_EQ(0, tree.Size());
	EXPECT_EQ(tree.begin(), tree.end());
	EXPECT_EQ(tree.Min(), tree.end());
	EXPECT_EQ(tree.Max(), tree.end());
	EXPECT_EQ(tree.Find(42), tree.end());
	EXPECT_EQ(tree.Predecessor(tree.end()), tree.end());
	EXPECT_EQ(tree.Successor(tree.end()), tree.end());

	tree.Delete(42);
	EXPECT_EQ(0, tree.Size());
}

TEST(BinarySearchTreeTest, InsertDuplicateValuesAndInOrderTraversal)
{
	guozi::tree::BinarySearchTree<int> tree;

	tree.Insert(5);
	tree.Insert(3);
	tree.Insert(7);
	tree.Insert(3);
	tree.Insert(5);
	tree.Insert(9);
	tree.Insert(1);

	EXPECT_EQ(5, tree.Size());
	EXPECT_EQ((std::vector<int> { 1, 3, 5, 7, 9 }), InOrderValues(tree));
	EXPECT_EQ(1, *tree.Min());
	EXPECT_EQ(9, *tree.Max());
}

TEST(BinarySearchTreeTest, NavigationPredecessorAndSuccessorBoundary)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 5, 3, 7, 2, 4, 6, 8 })
	{
		tree.Insert(value);
	}

	EXPECT_EQ(4, *tree.Predecessor(5));
	EXPECT_EQ(6, *tree.Successor(5));
	EXPECT_EQ(tree.end(), tree.Predecessor(2));
	EXPECT_EQ(tree.end(), tree.Successor(8));
}

TEST(BinarySearchTreeTest, DeleteLeafAndOneChildNode)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 5, 3, 7, 2, 4 })
	{
		tree.Insert(value);
	}

	tree.Delete(2);
	EXPECT_EQ(4, tree.Size());
	EXPECT_EQ((std::vector<int> { 3, 4, 5, 7 }), InOrderValues(tree));

	tree.Delete(3);
	EXPECT_EQ(3, tree.Size());
	EXPECT_EQ((std::vector<int> { 4, 5, 7 }), InOrderValues(tree));
	EXPECT_EQ(tree.end(), tree.Predecessor(4));
	EXPECT_EQ(5, *tree.Successor(4));
}

TEST(BinarySearchTreeTest, DeleteTwoChildrenNonRootNode)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 5, 3, 7, 2, 4, 6, 8 })
	{
		tree.Insert(value);
	}

	tree.Delete(3);
	EXPECT_EQ(6, tree.Size());
	EXPECT_EQ(tree.end(), tree.Find(3));
	EXPECT_EQ((std::vector<int> { 2, 4, 5, 6, 7, 8 }), InOrderValues(tree));
	EXPECT_EQ(2, *tree.Min());
	EXPECT_EQ(8, *tree.Max());
}

TEST(BinarySearchTreeTest, DeleteTwoChildrenRootNode)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 5, 3, 7, 2, 4, 6, 8 })
	{
		tree.Insert(value);
	}

	tree.Delete(5);
	EXPECT_EQ(6, tree.Size());
	EXPECT_EQ(tree.end(), tree.Find(5));
	EXPECT_EQ((std::vector<int> { 2, 3, 4, 6, 7, 8 }), InOrderValues(tree));
	EXPECT_EQ(2, *tree.Min());
	EXPECT_EQ(8, *tree.Max());
}

TEST(BinarySearchTreeTest, DeleteAllNodesToEmptyTree)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 4, 2, 6, 1, 3, 5, 7 })
	{
		tree.Insert(value);
	}

	for (int value : { 1, 3, 2, 5, 7, 6, 4 })
	{
		tree.Delete(value);
	}

	EXPECT_EQ(0, tree.Size());
	EXPECT_EQ(tree.begin(), tree.end());
	EXPECT_EQ(tree.Find(4), tree.end());
}

TEST(BinarySearchTreeTest, Select)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 5, 3, 7, 2, 4, 6, 8 })
	{
		tree.Insert(value);
	}

	EXPECT_EQ(tree.end(), tree.Select(-1));
	EXPECT_EQ(tree.end(), tree.Select(7));

	for (int i = 0; i < static_cast<int>(tree.Size()); ++i)
	{
		auto it = tree.Select(i);
		ASSERT_NE(tree.end(), it);
		EXPECT_EQ(i + 2, *it);
	}
}

TEST(BinarySearchTreeTest, Rank)
{
	guozi::tree::BinarySearchTree<int> tree;
	for (int value : { 5, 3, 7, 2, 4, 6, 8 })
	{
		tree.Insert(value);
	}

	EXPECT_EQ(-1, tree.Rank(1));
	EXPECT_EQ(0, tree.Rank(2));
	EXPECT_EQ(1, tree.Rank(3));
	EXPECT_EQ(2, tree.Rank(4));
	EXPECT_EQ(3, tree.Rank(5));
	EXPECT_EQ(4, tree.Rank(6));
	EXPECT_EQ(5, tree.Rank(7));
	EXPECT_EQ(6, tree.Rank(8));
	EXPECT_EQ(-1, tree.Rank(9));
}

TEST(BinarySearchTreeTest, LargeDatasetCoversPublicApisAndDeleteCases)
{
	guozi::tree::BinarySearchTree<int> tree;
	constexpr int kMinValue = 1;
	constexpr int kMaxValue = 63;
	constexpr int kProbeValue = 32;
	constexpr unsigned int kSeed = 20260320U;

	auto insertionOrder = BuildShuffledInsertionOrder(kMinValue, kMaxValue, kSeed);
	for (int value : insertionOrder)
	{
		tree.Insert(value);
	}

	std::vector<int> expected(static_cast<size_t>(kMaxValue - kMinValue + 1));
	std::iota(expected.begin(), expected.end(), kMinValue);
	ExpectTreeMatches(tree, expected);

	for (int duplicated : { kMinValue, kProbeValue, 48, kMaxValue })
	{
		tree.Insert(duplicated);
	}
	ExpectTreeMatches(tree, expected);

	EXPECT_NE(tree.end(), tree.Find(kProbeValue));
	EXPECT_EQ(tree.end(), tree.Find(0));
	EXPECT_EQ(tree.end(), tree.Find(100));

	EXPECT_EQ(tree.end(), tree.Predecessor(kMinValue));
	EXPECT_EQ(tree.end(), tree.Successor(kMaxValue));
	EXPECT_EQ(kProbeValue - 1, *tree.Predecessor(kProbeValue));
	EXPECT_EQ(kProbeValue + 1, *tree.Successor(kProbeValue));

	auto middle = tree.Find(kProbeValue);
	ASSERT_NE(tree.end(), middle);
	EXPECT_EQ(kProbeValue - 1, *tree.Predecessor(middle));
	EXPECT_EQ(kProbeValue + 1, *tree.Successor(middle));

	tree.Delete(1000);
	ExpectTreeMatches(tree, expected);

	tree.Delete(kMaxValue);
	EraseValue(expected, kMaxValue);
	ExpectTreeMatches(tree, expected);

	tree.Delete(kMaxValue - 1);
	EraseValue(expected, kMaxValue - 1);
	ExpectTreeMatches(tree, expected);

	tree.Delete(48);
	EraseValue(expected, 48);
	ExpectTreeMatches(tree, expected);

	tree.Delete(kProbeValue);
	EraseValue(expected, kProbeValue);
	ExpectTreeMatches(tree, expected);

	auto deletionOrder = expected;
	for (int value : deletionOrder)
	{
		tree.Delete(value);
	}
	expected.clear();
	ExpectTreeMatches(tree, expected);
	EXPECT_EQ(tree.end(), tree.Find(kMinValue));
	EXPECT_EQ(tree.end(), tree.Find(61));
}
