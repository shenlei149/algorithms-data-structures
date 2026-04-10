#include <gtest/gtest.h>

#include "hash/HashTable.h"

namespace
{
struct ConstantHash
{
	size_t operator()(int) const { return 0; }
};

template<typename Set>
void ExpectBasicSetBehavior()
{
	Set set(1);

	EXPECT_EQ(0, set.Size());
	EXPECT_FALSE(set.Contains(1));
	EXPECT_FALSE(set.Remove(1));

	set.Insert(1);
	set.Insert(9);
	set.Insert(17);

	EXPECT_EQ(3, set.Size());
	EXPECT_TRUE(set.Contains(1));
	EXPECT_TRUE(set.Contains(9));
	EXPECT_TRUE(set.Contains(17));
	EXPECT_FALSE(set.Contains(100));

	set.Insert(9);
	EXPECT_EQ(3, set.Size());

	EXPECT_TRUE(set.Remove(9));
	EXPECT_EQ(2, set.Size());
	EXPECT_FALSE(set.Contains(9));
	EXPECT_FALSE(set.Remove(9));
}

template<typename Set>
void ExpectResizeBehavior()
{
	Set set(1);
	size_t initialCapacity = set.Capacity();

	int insertedCount = 0;
	while (set.Capacity() == initialCapacity)
	{
		set.Insert(insertedCount);
		++insertedCount;
	}

	size_t grownCapacity = set.Capacity();
	EXPECT_GT(grownCapacity, initialCapacity);
	for (int i = 0; i < insertedCount; ++i)
	{
		EXPECT_TRUE(set.Contains(i));
	}

	int remainingCount = insertedCount;
	while (set.Capacity() == grownCapacity)
	{
		--remainingCount;
		EXPECT_TRUE(set.Remove(remainingCount));
	}

	EXPECT_LT(set.Capacity(), grownCapacity);
	for (int i = 0; i < remainingCount; ++i)
	{
		EXPECT_TRUE(set.Contains(i));
	}

	for (int i = remainingCount; i < insertedCount; ++i)
	{
		EXPECT_FALSE(set.Contains(i));
	}
}

template<typename Set>
void ExpectCollisionBehavior()
{
	Set set(1);

	set.Insert(1);
	set.Insert(2);
	set.Insert(3);
	set.Insert(4);

	EXPECT_EQ(4, set.Size());
	for (int key = 1; key <= 4; ++key)
	{
		EXPECT_TRUE(set.Contains(key));
	}

	set.Insert(3);
	EXPECT_EQ(4, set.Size());

	EXPECT_TRUE(set.Remove(2));
	EXPECT_FALSE(set.Contains(2));
	EXPECT_TRUE(set.Contains(1));
	EXPECT_TRUE(set.Contains(3));
	EXPECT_TRUE(set.Contains(4));
}
} // namespace

TEST(HashSetTest, ChainHashSetBasicOperations) { ExpectBasicSetBehavior<guozi::hash::ChainHashSet<int>>(); }

TEST(HashSetTest, DoubleHashSetBasicOperations) { ExpectBasicSetBehavior<guozi::hash::DoubleHashSet<int>>(); }

TEST(HashSetTest, ChainHashSetResizesAndPreservesMembers) { ExpectResizeBehavior<guozi::hash::ChainHashSet<int>>(); }

TEST(HashSetTest, DoubleHashSetResizesAndPreservesMembers) { ExpectResizeBehavior<guozi::hash::DoubleHashSet<int>>(); }

TEST(HashSetTest, ChainHashSetHandlesCollisions)
{
	ExpectCollisionBehavior<
		guozi::hash::HashTable<int, std::monostate, guozi::hash::ChainPolicy<int, std::monostate>, ConstantHash>>();
}

TEST(HashSetTest, DoubleHashSetHandlesCollisions)
{
	ExpectCollisionBehavior<
		guozi::hash::
			HashTable<int, std::monostate, guozi::hash::DoubleHashPolicy<int, std::monostate>, ConstantHash>>();
}
