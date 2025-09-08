#include "InternalSort.h"

#include <random>

#include <gtest/gtest.h>

#include "common/Utils.h"

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
