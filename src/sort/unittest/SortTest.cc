#include "InternalSort.h"

#include <random>

#include <gtest/gtest.h>

#include "common/Utils.h"

TEST(Sort, IsSorted)
{
	std::vector<int> data1 { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<int> data2 { 2, 1, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<int> data3 { 2, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<int> data4 { 1, 2, 5, 5, 5, 5, 7, 8, 9 };
	std::vector<int> data5 { 1, 2, 3, 4, 5, 6, 7, 9, 8 };
	ASSERT_TRUE(guozi::utils::IsSorted(data1.begin(), data1.end()));
	ASSERT_FALSE(guozi::utils::IsSorted(data2.begin(), data2.end()));
	ASSERT_TRUE(guozi::utils::IsSorted(data3.begin(), data3.end()));
	ASSERT_TRUE(guozi::utils::IsSorted(data4.begin(), data4.end()));
	ASSERT_FALSE(guozi::utils::IsSorted(data5.begin(), data5.end()));
}

using DataType = typename std::vector<int>;
using IteratorType = typename DataType::iterator;
using FunctionType = typename std::function<void(IteratorType, IteratorType)>;

static constexpr std::array<size_t, 12> ArrayLength { 0, 1, 2, 3, 5, 7, 11, 13, 100, 101, 10000, 10001 };

static const FunctionType SelectionSort = [](IteratorType begin, IteratorType end)
{
	guozi::sort::SelectionSort(begin, end);
};

static const FunctionType InsertionSort = [](IteratorType begin, IteratorType end)
{
	guozi::sort::InsertionSort(begin, end);
};

static const FunctionType MergeSort = [](IteratorType begin, IteratorType end)
{
	guozi::sort::MergeSort(begin, end);
};

static const std::array<FunctionType, 3> Functions { SelectionSort, InsertionSort, MergeSort };

inline DataType
GenerateData(size_t length)
{
	DataType data;
	data.resize(length);

	std::random_device rd {};
	std::mt19937 gen { rd() };
	std::uniform_int_distribution<int> dist(0, 2 * length);
	for (size_t i = 0; i < length; i++)
	{
		data[i] = dist(gen);
	}

	return data;
}

TEST(Sort, AllInOne)
{
	for (auto &&length : ArrayLength)
	{
		for (auto &&func : Functions)
		{
			auto data = GenerateData(length);
			func(data.begin(), data.end());
			ASSERT_TRUE(guozi::utils::IsSorted(data.begin(), data.end()));
		}
	}
}
