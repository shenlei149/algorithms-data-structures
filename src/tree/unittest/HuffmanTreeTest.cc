#include "tree/Huffman.h"
#include <gtest/gtest.h>

using namespace guozi::tree;

TEST(HuffmanTreeTest, BasicCase1)
{
	std::unordered_map<char, double> weights = {
		{ 'A', 0.32 },
		{ 'B', 0.25 },
		{ 'C', 0.20 },
		{ 'D', 0.18 },
		{ 'E', 0.05 }
	};

	HuffmanTree<char, double> huffman(weights);
	const auto &table = huffman.GetEncodingTable();

	EXPECT_EQ(table.size(), 5);

	// In optimal Huffman for these weights:
	// Length of A (0.32): 2 bits
	// Length of B (0.25): 2 bits
	// Length of C (0.20): 2 bits
	// Length of D (0.18): 3 bits
	// Length of E (0.05): 3 bits

	EXPECT_EQ(table.at('A').size(), 2);
	EXPECT_EQ(table.at('B').size(), 2);
	EXPECT_EQ(table.at('C').size(), 2);
	EXPECT_EQ(table.at('D').size(), 3);
	EXPECT_EQ(table.at('E').size(), 3);

	// Average code length = 0.32*2 + 0.25*2 + 0.20*2 + 0.18*3 + 0.05*3 = 0.64 + 0.50 + 0.40 + 0.54 + 0.15 = 2.23
	EXPECT_NEAR(huffman.GetAverageCodeLength(), 2.23, 1e-6);
}

TEST(HuffmanTreeTest, BasicCase2)
{
	std::unordered_map<char, double> weights = {
		{ 'A', 0.16 },
		{ 'B', 0.08 },
		{ 'C', 0.35 },
		{ 'D', 0.07 },
		{ 'E', 0.34 }
	};

	HuffmanTree<char, double> huffman(weights);
	const auto &table = huffman.GetEncodingTable();

	EXPECT_EQ(table.size(), 5);

	// Elements sorted: D(0.07), B(0.08), A(0.16), E(0.34), C(0.35)
	// D+B = 0.15
	// A+0.15 = 0.31
	// E+0.31 = 0.65
	// C+0.65 = 1.00
	// Wait, standard two queue:
	// qLeaf: D(0.07), B(0.08), A(0.16), E(0.34), C(0.35)
	// Merge D and B -> N1(0.15).
	// Next min: N1(0.15) and A(0.16) -> N2(0.31).
	// Next min: N2(0.31) and E(0.34) -> N3(0.65).
	// Next min: C(0.35) and N3(0.65) -> Root(1.00).

	// Expected lengths:
	// C: 1 bit (0.35)
	// E: 2 bits (0.34)
	// A: 3 bits (0.16)
	// B: 4 bits (0.08)
	// D: 4 bits (0.07)

	EXPECT_EQ(table.at('C').size(), 1);
	EXPECT_EQ(table.at('E').size(), 2);
	EXPECT_EQ(table.at('A').size(), 3);
	EXPECT_EQ(table.at('B').size(), 4);
	EXPECT_EQ(table.at('D').size(), 4);

	// Average code length = 0.35*1 + 0.34*2 + 0.16*3 + 0.08*4 + 0.07*4
	// = 0.35 + 0.68 + 0.48 + 0.32 + 0.28 = 2.11
	EXPECT_NEAR(huffman.GetAverageCodeLength(), 2.11, 1e-6);
}

TEST(HuffmanTreeTest, EmptyTree)
{
	std::unordered_map<char, double> emptyWeights;
	HuffmanTree<char, double> huffman(emptyWeights);

	EXPECT_TRUE(huffman.GetEncodingTable().empty());
	EXPECT_DOUBLE_EQ(huffman.GetAverageCodeLength(), 0.0);
}

TEST(HuffmanTreeTest, SingleNode)
{
	std::unordered_map<char, double> weights = {
		{ 'A', 1.0 }
	};
	HuffmanTree<char, double> huffman(weights);

	const auto &table = huffman.GetEncodingTable();
	EXPECT_EQ(table.size(), 1);

	// A single node tree doesn't require any branching, its path is empty string length 0.
	EXPECT_EQ(table.at('A').size(), 0);
	EXPECT_DOUBLE_EQ(huffman.GetAverageCodeLength(), 0.0);
}

TEST(HuffmanTreeTest, UniformWeights)
{
	std::unordered_map<char, int> weights = {
		{ 'A', 1 },
		{ 'B', 1 },
		{ 'C', 1 },
		{ 'D', 1 },
		{ 'E', 1 },
		{ 'F', 1 },
		{ 'G', 1 },
		{ 'H', 1 }
	};
	// 8 elements with equal weight should form a perfectly balanced tree.
	// All leaves should be at depth 3.
	HuffmanTree<char, int> huffman(weights);
	const auto &table = huffman.GetEncodingTable();

	for (char c = 'A'; c <= 'H'; ++c)
	{
		EXPECT_EQ(table.at(c).size(), 3);
	}
	EXPECT_DOUBLE_EQ(huffman.GetAverageCodeLength(), 3.0);
}
