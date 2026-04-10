#include <gtest/gtest.h>

#include "hash/HashTable.h"

#include <string>

namespace
{
struct ConstantHash
{
	size_t operator()(int) const { return 0; }
};

template<typename Map>
void ExpectBasicMapBehavior()
{
	Map map(1);

	EXPECT_EQ(0, map.Size());
	EXPECT_FALSE(map.Contains(1));
	EXPECT_EQ(nullptr, map.Get(1));
	EXPECT_FALSE(map.Remove(1));

	map.Insert(1, "one");
	map.Insert(9, "nine");
	map.Insert(17, "seventeen");

	ASSERT_EQ(3, map.Size());
	ASSERT_NE(nullptr, map.Get(1));
	EXPECT_EQ("one", *map.Get(1));
	ASSERT_NE(nullptr, map.Get(9));
	EXPECT_EQ("nine", *map.Get(9));
	ASSERT_NE(nullptr, map.Get(17));
	EXPECT_EQ("seventeen", *map.Get(17));

	map.Insert(9, "NINE");
	ASSERT_EQ(3, map.Size());
	ASSERT_NE(nullptr, map.Get(9));
	EXPECT_EQ("NINE", *map.Get(9));

	const auto &constMap = map;
	ASSERT_NE(nullptr, constMap.Get(1));
	EXPECT_EQ("one", *constMap.Get(1));
	EXPECT_EQ(nullptr, constMap.Get(100));

	EXPECT_TRUE(map.Remove(9));
	EXPECT_EQ(2, map.Size());
	EXPECT_FALSE(map.Contains(9));
	EXPECT_EQ(nullptr, map.Get(9));
	EXPECT_FALSE(map.Remove(9));
}

template<typename Map>
void ExpectResizeBehavior()
{
	Map map(1);
	size_t initialCapacity = map.Capacity();

	int insertedCount = 0;
	while (map.Capacity() == initialCapacity)
	{
		map.Insert(insertedCount, insertedCount * 10);
		++insertedCount;
	}

	size_t grownCapacity = map.Capacity();
	EXPECT_GT(grownCapacity, initialCapacity);
	for (int i = 0; i < insertedCount; ++i)
	{
		ASSERT_NE(nullptr, map.Get(i));
		EXPECT_EQ(i * 10, *map.Get(i));
	}

	int remainingCount = insertedCount;
	while (map.Capacity() == grownCapacity)
	{
		--remainingCount;
		EXPECT_TRUE(map.Remove(remainingCount));
	}

	EXPECT_LT(map.Capacity(), grownCapacity);
	for (int i = 0; i < remainingCount; ++i)
	{
		ASSERT_NE(nullptr, map.Get(i));
		EXPECT_EQ(i * 10, *map.Get(i));
	}

	for (int i = remainingCount; i < insertedCount; ++i)
	{
		EXPECT_EQ(nullptr, map.Get(i));
	}
}

template<typename Map>
void ExpectCollisionBehavior()
{
	Map map(1);

	map.Insert(1, 10);
	map.Insert(2, 20);
	map.Insert(3, 30);
	map.Insert(4, 40);

	EXPECT_EQ(4, map.Size());
	for (int key = 1; key <= 4; ++key)
	{
		ASSERT_NE(nullptr, map.Get(key));
		EXPECT_EQ(key * 10, *map.Get(key));
	}

	map.Insert(3, 300);
	EXPECT_EQ(4, map.Size());
	ASSERT_NE(nullptr, map.Get(3));
	EXPECT_EQ(300, *map.Get(3));

	EXPECT_TRUE(map.Remove(2));
	EXPECT_EQ(nullptr, map.Get(2));
	ASSERT_NE(nullptr, map.Get(1));
	EXPECT_EQ(10, *map.Get(1));
	ASSERT_NE(nullptr, map.Get(3));
	EXPECT_EQ(300, *map.Get(3));
	ASSERT_NE(nullptr, map.Get(4));
	EXPECT_EQ(40, *map.Get(4));
}
} // namespace

TEST(HashMapTest, ChainHashMapBasicOperations)
{
	ExpectBasicMapBehavior<guozi::hash::ChainHashMap<int, std::string>>();
}

TEST(HashMapTest, DoubleHashMapBasicOperations)
{
	ExpectBasicMapBehavior<guozi::hash::DoubleHashMap<int, std::string>>();
}

TEST(HashMapTest, ChainHashMapResizesAndPreservesEntries)
{
	ExpectResizeBehavior<guozi::hash::ChainHashMap<int, int>>();
}

TEST(HashMapTest, DoubleHashMapResizesAndPreservesEntries)
{
	ExpectResizeBehavior<guozi::hash::DoubleHashMap<int, int>>();
}

TEST(HashMapTest, ChainHashMapHandlesCollisions)
{
	ExpectCollisionBehavior<guozi::hash::HashTable<int, int, guozi::hash::ChainPolicy<int, int>, ConstantHash>>();
}

TEST(HashMapTest, DoubleHashMapHandlesCollisions)
{
	ExpectCollisionBehavior<guozi::hash::HashTable<int, int, guozi::hash::DoubleHashPolicy<int, int>, ConstantHash>>();
}
