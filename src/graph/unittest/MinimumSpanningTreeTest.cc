#include <algorithm>
#include <cmath>

#include <gtest/gtest.h>

#include "Factory.h"
#include "graph/MinimumSpanningTree.h"

using namespace guozi::graph;

TEST(MinimumSpanningTreeTest, PrimFindsMinimumSpanningTree)
{
	auto graph = CreateWeightedUndirectedGraph();
	auto mst = PrimMinimumSpanningTree<decltype(graph), double>(graph);

	auto edges = mst.EdgeIds();
	std::sort(edges.begin(), edges.end());
	EXPECT_EQ(7, edges.size());
	EXPECT_EQ((std::vector<size_t> { 0, 2, 3, 6, 7, 8, 12 }), edges);
	EXPECT_TRUE(std::abs(mst.Weight() - 1.81) < 1e-9);
}

TEST(MinimumSpanningTreeTest, KruskalFindsMinimumSpanningTree)
{
	auto graph = CreateWeightedUndirectedGraph();
	auto mst = KruskalMinimumSpanningTree<decltype(graph), double>(graph);

	auto edges = mst.EdgeIds();
	std::sort(edges.begin(), edges.end());
	EXPECT_EQ(7, edges.size());
	EXPECT_EQ((std::vector<size_t> { 0, 2, 3, 6, 7, 8, 12 }), edges);
	EXPECT_TRUE(std::abs(mst.Weight() - 1.81) < 1e-9);
}
