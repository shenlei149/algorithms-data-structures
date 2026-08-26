#include <gtest/gtest.h>

#include "Factory.h"
#include "graph/Topology.h"

using namespace guozi::graph;

TEST(TopologyTest, ProducesTopologicalOrder)
{
	auto graph = CreateDirectedGraph();
	Topology topology(graph);

	auto order = topology.Order();
	auto expected = std::vector<size_t> { 7, 8, 6, 9, 11, 10, 12, 0, 5, 4, 2, 3, 1 };
	EXPECT_EQ(expected, std::vector<size_t>(order.begin(), order.end()));
}

TEST(TopologyTest, HandlesEmptyGraph)
{
	DirectedGraph<> graph;
	Topology topology(graph);

	EXPECT_TRUE(topology.Order().empty());
}
