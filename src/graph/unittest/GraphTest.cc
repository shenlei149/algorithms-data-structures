#include <gtest/gtest.h>

#include "Graph.h"
#include "Path.h"

using namespace guozi::graph;

Graph<> CreateGraph1()
{
	Graph<> graph(false /* undirected */);
	for (size_t i = 0; i < 13; i++)
	{
		graph.AddVertex();
	}

	graph.AddEdge(0, 5);
	graph.AddEdge(4, 3);
	graph.AddEdge(0, 1);
	graph.AddEdge(9, 12);
	graph.AddEdge(6, 4);
	graph.AddEdge(5, 4);
	graph.AddEdge(0, 2);
	graph.AddEdge(11, 12);
	graph.AddEdge(9, 10);
	graph.AddEdge(0, 6);
	graph.AddEdge(7, 8);
	graph.AddEdge(9, 11);
	graph.AddEdge(5, 3);

	return graph;
}

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

	ASSERT_EQ((std::vector<size_t> { e1, e2 }), cityMap.GetOutgoingEdgeIndices(bj));
	ASSERT_EQ((std::vector<size_t> { e1, e3 }), cityMap.GetOutgoingEdgeIndices(sh));
	ASSERT_EQ((std::vector<size_t> { e2, e4 }), cityMap.GetOutgoingEdgeIndices(gz));
	ASSERT_EQ((std::vector<size_t> { e3, e4 }), cityMap.GetOutgoingEdgeIndices(sz));
}

TEST(GraphTest, Path)
{
	auto graph = CreateGraph1();
	ASSERT_EQ(13, graph.GetVertexCount());
	ASSERT_EQ(13, graph.GetEdgeCount());

	Path path1(graph, 0);

	ASSERT_TRUE(path1.HasPathTo(0));
	ASSERT_TRUE(path1.HasPathTo(1));
	ASSERT_TRUE(path1.HasPathTo(2));
	ASSERT_TRUE(path1.HasPathTo(3));
	ASSERT_TRUE(path1.HasPathTo(4));
	ASSERT_TRUE(path1.HasPathTo(5));
	ASSERT_TRUE(path1.HasPathTo(6));

	ASSERT_FALSE(path1.HasPathTo(7));
	ASSERT_FALSE(path1.HasPathTo(8));
	ASSERT_FALSE(path1.HasPathTo(9));
	ASSERT_FALSE(path1.HasPathTo(10));
	ASSERT_FALSE(path1.HasPathTo(11));
	ASSERT_FALSE(path1.HasPathTo(12));

	EXPECT_EQ((std::vector<size_t> { 0 }), static_cast<std::vector<size_t>>(path1.PathTo(0)));
	EXPECT_EQ((std::vector<size_t> { 0, 1 }), static_cast<std::vector<size_t>>(path1.PathTo(1)));
	EXPECT_EQ((std::vector<size_t> { 0, 2 }), static_cast<std::vector<size_t>>(path1.PathTo(2)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 3 }), static_cast<std::vector<size_t>>(path1.PathTo(3)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 4 }), static_cast<std::vector<size_t>>(path1.PathTo(4)));
	EXPECT_EQ((std::vector<size_t> { 0, 5 }), static_cast<std::vector<size_t>>(path1.PathTo(5)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path1.PathTo(7)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path1.PathTo(11)));

	Path path2(graph, 9);

	ASSERT_TRUE(path2.HasPathTo(9));
	ASSERT_TRUE(path2.HasPathTo(10));
	ASSERT_TRUE(path2.HasPathTo(11));
	ASSERT_TRUE(path2.HasPathTo(12));

	ASSERT_FALSE(path2.HasPathTo(0));
	ASSERT_FALSE(path2.HasPathTo(1));
	ASSERT_FALSE(path2.HasPathTo(2));
	ASSERT_FALSE(path2.HasPathTo(3));
	ASSERT_FALSE(path2.HasPathTo(4));
	ASSERT_FALSE(path2.HasPathTo(5));
	ASSERT_FALSE(path2.HasPathTo(6));
	ASSERT_FALSE(path2.HasPathTo(7));
	ASSERT_FALSE(path2.HasPathTo(8));

	EXPECT_EQ((std::vector<size_t> { 9 }), static_cast<std::vector<size_t>>(path2.PathTo(9)));
	EXPECT_EQ((std::vector<size_t> { 9, 10 }), static_cast<std::vector<size_t>>(path2.PathTo(10)));
	EXPECT_EQ((std::vector<size_t> { 9, 11 }), static_cast<std::vector<size_t>>(path2.PathTo(11)));
	EXPECT_EQ((std::vector<size_t> { 9, 12 }), static_cast<std::vector<size_t>>(path2.PathTo(12)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path2.PathTo(7)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path2.PathTo(3)));
}
