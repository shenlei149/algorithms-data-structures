#include "sort/InternalSort.h"

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
	EXPECT_TRUE(guozi::utils::IsSorted(data1.begin(), data1.end()));
	EXPECT_FALSE(guozi::utils::IsSorted(data2.begin(), data2.end()));
	EXPECT_TRUE(guozi::utils::IsSorted(data3.begin(), data3.end()));
	EXPECT_TRUE(guozi::utils::IsSorted(data4.begin(), data4.end()));
	EXPECT_FALSE(guozi::utils::IsSorted(data5.begin(), data5.end()));
}

TEST(Sort, CountInversion)
{
	std::vector<int> example { 1, 3, 5, 2, 4, 6 };
	EXPECT_EQ(3, guozi::sort::CountInversion(example.begin(), example.end()));

	std::vector<int> data { 3, 4, 2, 8, 6, 7, 1, 5, 9 };
	EXPECT_EQ(13, guozi::sort::CountInversion(data.begin(), data.end()));
}

using DataType = typename std::vector<int>;
using IteratorType = typename DataType::iterator;
using FunctionType = typename std::function<void(IteratorType, IteratorType)>;

static constexpr std::array<size_t, 12> ArrayLength { 0, 1, 2, 3, 5, 7, 11, 13, 100, 101, 10000, 10001 };

static const FunctionType SelectionSort = [](IteratorType begin, IteratorType end)
{ guozi::sort::SelectionSort(begin, end); };

static const FunctionType InsertionSort = [](IteratorType begin, IteratorType end)
{ guozi::sort::InsertionSort(begin, end); };

static const FunctionType MergeSort = [](IteratorType begin, IteratorType end) { guozi::sort::MergeSort(begin, end); };

static const FunctionType QuickSort = [](IteratorType begin, IteratorType end) { guozi::sort::QuickSort(begin, end); };

static const FunctionType HeapSort = [](IteratorType begin, IteratorType end) { guozi::sort::HeapSort(begin, end); };

static const std::array<FunctionType, 5> Functions { SelectionSort, InsertionSort, MergeSort, QuickSort, HeapSort };

inline DataType GenerateData(size_t length)
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
			EXPECT_TRUE(guozi::utils::IsSorted(data.begin(), data.end()));
		}
	}
}
