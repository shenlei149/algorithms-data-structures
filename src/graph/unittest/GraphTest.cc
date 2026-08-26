#include <gtest/gtest.h>

#include "graph/Graph.h"

using namespace guozi::graph;

TEST(GraphTest, Basic)
{
	UndirectedGraph<std::string, int> cityMap;

	auto bj = cityMap.AddVertex("Beijing");
	auto sh = cityMap.AddVertex("Shanghai");
	auto gz = cityMap.AddVertex("Guangzhou");
	auto sz = cityMap.AddVertex("Shenzhen");

	auto e1 = cityMap.AddEdge(bj, sh, 1200);
	auto e2 = cityMap.AddEdge(bj, gz, 2100);
	auto e3 = cityMap.AddEdge(sh, sz, 1400);
	auto e4 = cityMap.AddEdge(gz, sz, 100);

	EXPECT_EQ(4, cityMap.VertexCount());
	EXPECT_EQ(4, cityMap.EdgeCount());
	EXPECT_EQ("Beijing", cityMap.GetVertex(bj));
	EXPECT_EQ("Shanghai", cityMap.GetVertex(sh));
	EXPECT_EQ("Guangzhou", cityMap.GetVertex(gz));
	EXPECT_EQ("Shenzhen", cityMap.GetVertex(sz));
	EXPECT_EQ(1200, cityMap.GetEdge(e1));
	EXPECT_EQ(2100, cityMap.GetEdge(e2));
	EXPECT_EQ(1400, cityMap.GetEdge(e3));
	EXPECT_EQ(100, cityMap.GetEdge(e4));
	EXPECT_EQ((std::vector<size_t> { e1, e2 }), cityMap.OutgoingEdgeIndices(bj));
	EXPECT_EQ((std::vector<size_t> { e1, e3 }), cityMap.OutgoingEdgeIndices(sh));
	EXPECT_EQ((std::vector<size_t> { e2, e4 }), cityMap.OutgoingEdgeIndices(gz));
	EXPECT_EQ((std::vector<size_t> { e3, e4 }), cityMap.OutgoingEdgeIndices(sz));
	EXPECT_EQ((std::vector<size_t> { e1, e2 }), cityMap.IncomingEdgeIndices(bj));
	EXPECT_EQ((std::vector<size_t> { e1, e3 }), cityMap.IncomingEdgeIndices(sh));
	EXPECT_EQ((std::vector<size_t> { e2, e4 }), cityMap.IncomingEdgeIndices(gz));
	EXPECT_EQ((std::vector<size_t> { e3, e4 }), cityMap.IncomingEdgeIndices(sz));
}

TEST(GraphTest, DirectedGraphTracksDirectionsAndTranspose)
{
	DirectedGraph<std::string, int> graph;
	auto a = graph.AddVertex("a");
	auto b = graph.AddVertex("b");
	auto c = graph.AddVertex("c");
	auto edge = graph.AddEdge(a, b, 7);
	graph.AddEdge(b, c, 11);

	EXPECT_EQ((std::vector<size_t> { edge }), graph.OutgoingEdgeIndices(a));
	EXPECT_TRUE(graph.IncomingEdgeIndices(a).empty());
	EXPECT_EQ((std::vector<size_t> { edge }), graph.IncomingEdgeIndices(b));
	EXPECT_EQ(a, graph.GetSrcVertexId(edge));
	EXPECT_EQ(b, graph.GetDstVertexId(edge));

	auto transpose = graph.Transpose();
	EXPECT_EQ(graph.VertexCount(), transpose.VertexCount());
	EXPECT_EQ(graph.EdgeCount(), transpose.EdgeCount());
	EXPECT_EQ("a", transpose.GetVertex(a));
	EXPECT_EQ("b", transpose.GetVertex(b));
	EXPECT_EQ("c", transpose.GetVertex(c));
	EXPECT_EQ(b, transpose.GetSrcVertexId(edge));
	EXPECT_EQ(a, transpose.GetDstVertexId(edge));
	EXPECT_EQ(7, transpose.GetEdge(edge));
}
