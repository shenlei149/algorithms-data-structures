#include <gtest/gtest.h>

#include "ConnectedComponent.h"
#include "Graph.h"
#include "Path.h"

using namespace guozi::graph;

UndirectedGraph<> CreateGraph1()
{
	UndirectedGraph<> graph;
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
}

TEST(GraphTest, Path)
{
	auto graph = CreateGraph1();
	EXPECT_EQ(13, graph.VertexCount());
	EXPECT_EQ(13, graph.EdgeCount());

	Path path1(graph, 0);

	EXPECT_TRUE(path1.HasPathTo(0));
	EXPECT_TRUE(path1.HasPathTo(1));
	EXPECT_TRUE(path1.HasPathTo(2));
	EXPECT_TRUE(path1.HasPathTo(3));
	EXPECT_TRUE(path1.HasPathTo(4));
	EXPECT_TRUE(path1.HasPathTo(5));
	EXPECT_TRUE(path1.HasPathTo(6));

	EXPECT_FALSE(path1.HasPathTo(7));
	EXPECT_FALSE(path1.HasPathTo(8));
	EXPECT_FALSE(path1.HasPathTo(9));
	EXPECT_FALSE(path1.HasPathTo(10));
	EXPECT_FALSE(path1.HasPathTo(11));
	EXPECT_FALSE(path1.HasPathTo(12));

	EXPECT_EQ((std::vector<size_t> { 0 }), static_cast<std::vector<size_t>>(path1.PathTo(0)));
	EXPECT_EQ((std::vector<size_t> { 0, 1 }), static_cast<std::vector<size_t>>(path1.PathTo(1)));
	EXPECT_EQ((std::vector<size_t> { 0, 2 }), static_cast<std::vector<size_t>>(path1.PathTo(2)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 3 }), static_cast<std::vector<size_t>>(path1.PathTo(3)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 4 }), static_cast<std::vector<size_t>>(path1.PathTo(4)));
	EXPECT_EQ((std::vector<size_t> { 0, 5 }), static_cast<std::vector<size_t>>(path1.PathTo(5)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path1.PathTo(7)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path1.PathTo(11)));

	Path path2(graph, 9);

	EXPECT_TRUE(path2.HasPathTo(9));
	EXPECT_TRUE(path2.HasPathTo(10));
	EXPECT_TRUE(path2.HasPathTo(11));
	EXPECT_TRUE(path2.HasPathTo(12));

	EXPECT_FALSE(path2.HasPathTo(0));
	EXPECT_FALSE(path2.HasPathTo(1));
	EXPECT_FALSE(path2.HasPathTo(2));
	EXPECT_FALSE(path2.HasPathTo(3));
	EXPECT_FALSE(path2.HasPathTo(4));
	EXPECT_FALSE(path2.HasPathTo(5));
	EXPECT_FALSE(path2.HasPathTo(6));
	EXPECT_FALSE(path2.HasPathTo(7));
	EXPECT_FALSE(path2.HasPathTo(8));

	EXPECT_EQ((std::vector<size_t> { 9 }), static_cast<std::vector<size_t>>(path2.PathTo(9)));
	EXPECT_EQ((std::vector<size_t> { 9, 10 }), static_cast<std::vector<size_t>>(path2.PathTo(10)));
	EXPECT_EQ((std::vector<size_t> { 9, 11 }), static_cast<std::vector<size_t>>(path2.PathTo(11)));
	EXPECT_EQ((std::vector<size_t> { 9, 12 }), static_cast<std::vector<size_t>>(path2.PathTo(12)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path2.PathTo(7)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path2.PathTo(3)));
}

TEST(GraphTest, UndirectedConnectedComponent)
{
	auto graph = CreateGraph1();
	auto cc = UndirectedConnectedComponent(graph);

	EXPECT_EQ(3, cc.Count());

	EXPECT_TRUE(cc.Connected(0, 0));
	EXPECT_TRUE(cc.Connected(1, 0));
	EXPECT_TRUE(cc.Connected(2, 0));
	EXPECT_TRUE(cc.Connected(3, 0));
	EXPECT_TRUE(cc.Connected(4, 0));
	EXPECT_TRUE(cc.Connected(5, 0));
	EXPECT_TRUE(cc.Connected(6, 0));

	EXPECT_FALSE(cc.Connected(0, 7));
	EXPECT_FALSE(cc.Connected(0, 8));
	EXPECT_FALSE(cc.Connected(0, 9));
	EXPECT_FALSE(cc.Connected(0, 10));
	EXPECT_FALSE(cc.Connected(0, 11));
	EXPECT_FALSE(cc.Connected(0, 12));

	EXPECT_TRUE(cc.Connected(7, 8));
	EXPECT_FALSE(cc.Connected(7, 9));

	EXPECT_TRUE(cc.Connected(9, 10));
	EXPECT_TRUE(cc.Connected(11, 12));

	EXPECT_EQ(0, cc.Id(0));
	EXPECT_EQ(0, cc.Id(1));
	EXPECT_EQ(0, cc.Id(2));
	EXPECT_EQ(0, cc.Id(3));
	EXPECT_EQ(0, cc.Id(4));
	EXPECT_EQ(0, cc.Id(5));
	EXPECT_EQ(0, cc.Id(6));

	EXPECT_EQ(1, cc.Id(7));
	EXPECT_EQ(1, cc.Id(8));

	EXPECT_EQ(2, cc.Id(9));
	EXPECT_EQ(2, cc.Id(10));
	EXPECT_EQ(2, cc.Id(11));
	EXPECT_EQ(2, cc.Id(12));
}
