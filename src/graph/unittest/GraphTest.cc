#include "Graph.h"

#include <gtest/gtest.h>

TEST(GraphTest, Basic)
{
	Graph<std::string, int> cityMap(false /* undirected */);

	auto bj = cityMap.AddVertex("Beijing");
	auto sh = cityMap.AddVertex("Shanghai");
	auto gz = cityMap.AddVertex("Guangzhou");
	auto sz = cityMap.AddVertex("Shenzhen");

	auto e1 = cityMap.AddEdge(bj, sh, 1200);
	auto e2 = cityMap.AddEdge(bj, gz, 2100);
	auto e3 = cityMap.AddEdge(sh, sz, 1400);
	auto e4 = cityMap.AddEdge(gz, sz, 100);

	ASSERT_EQ(4, cityMap.GetVertexCount());
	ASSERT_EQ(4, cityMap.GetEdgeCount());

	ASSERT_EQ("Beijing", cityMap.GetVertex(bj));
	ASSERT_EQ("Shanghai", cityMap.GetVertex(sh));
	ASSERT_EQ("Guangzhou", cityMap.GetVertex(gz));
	ASSERT_EQ("Shenzhen", cityMap.GetVertex(sz));

	ASSERT_EQ(1200, cityMap.GetEdge(e1));
	ASSERT_EQ(2100, cityMap.GetEdge(e2));
	ASSERT_EQ(1400, cityMap.GetEdge(e3));
	ASSERT_EQ(100, cityMap.GetEdge(e4));

	ASSERT_EQ((std::vector<size_t> { e1, e2 }), cityMap.GetOutgoingEdges(bj));
	ASSERT_EQ((std::vector<size_t> { e1, e3 }), cityMap.GetOutgoingEdges(sh));
	ASSERT_EQ((std::vector<size_t> { e2, e4 }), cityMap.GetOutgoingEdges(gz));
	ASSERT_EQ((std::vector<size_t> { e3, e4 }), cityMap.GetOutgoingEdges(sz));
}
