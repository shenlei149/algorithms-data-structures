#include <cmath>

#include <gtest/gtest.h>

#include "Factory.h"
#include "graph/ConnectedComponent.h"
#include "graph/Path.h"
#include "graph/ShortestPath.h"
#include "graph/Topology.h"

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
	auto graph = CreateUndirectedGraph();
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

	EXPECT_EQ((std::vector<size_t> { 0 }), static_cast<std::vector<size_t>>(path1.GetPathTo(0)));
	EXPECT_EQ((std::vector<size_t> { 0, 1 }), static_cast<std::vector<size_t>>(path1.GetPathTo(1)));
	EXPECT_EQ((std::vector<size_t> { 0, 2 }), static_cast<std::vector<size_t>>(path1.GetPathTo(2)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 3 }), static_cast<std::vector<size_t>>(path1.GetPathTo(3)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 4 }), static_cast<std::vector<size_t>>(path1.GetPathTo(4)));
	EXPECT_EQ((std::vector<size_t> { 0, 5 }), static_cast<std::vector<size_t>>(path1.GetPathTo(5)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path1.GetPathTo(7)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path1.GetPathTo(11)));

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

	EXPECT_EQ((std::vector<size_t> { 9 }), static_cast<std::vector<size_t>>(path2.GetPathTo(9)));
	EXPECT_EQ((std::vector<size_t> { 9, 10 }), static_cast<std::vector<size_t>>(path2.GetPathTo(10)));
	EXPECT_EQ((std::vector<size_t> { 9, 11 }), static_cast<std::vector<size_t>>(path2.GetPathTo(11)));
	EXPECT_EQ((std::vector<size_t> { 9, 12 }), static_cast<std::vector<size_t>>(path2.GetPathTo(12)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path2.GetPathTo(7)));
	EXPECT_EQ((std::vector<size_t> {}), static_cast<std::vector<size_t>>(path2.GetPathTo(3)));
}

TEST(GraphTest, UndirectedConnectedComponent)
{
	auto graph = CreateUndirectedGraph();
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

TEST(GraphTest, StronglyConnectedComponent)
{
	auto graph = CreateDirectedGraph();
	StronglyConnectedComponent scc(graph);

	EXPECT_EQ(5, scc.Count());

	EXPECT_EQ(0, scc.Id(1));

	EXPECT_EQ(1, scc.Id(0));
	EXPECT_EQ(1, scc.Id(5));
	EXPECT_EQ(1, scc.Id(4));
	EXPECT_EQ(1, scc.Id(3));
	EXPECT_EQ(1, scc.Id(2));

	EXPECT_EQ(2, scc.Id(11));
	EXPECT_EQ(2, scc.Id(12));
	EXPECT_EQ(2, scc.Id(9));
	EXPECT_EQ(2, scc.Id(10));

	EXPECT_EQ(3, scc.Id(6));

	EXPECT_EQ(4, scc.Id(8));
	EXPECT_EQ(4, scc.Id(7));
}

TEST(GraphTest, Topology)
{
	auto graph = CreateDirectedGraph();
	Topology topology(graph);

	auto order = topology.Order();
	auto expected = std::vector<size_t> { 7, 8, 6, 9, 11, 10, 12, 0, 5, 4, 2, 3, 1 };
	EXPECT_EQ(expected, std::vector<size_t>(order.begin(), order.end()));
}

TEST(GraphTest, Dijkstra)
{
	auto graph = CreateWeightedDirectedGraph();
	auto shortestPath = DijkstraShortestPath<decltype(graph), double>(graph, 0);

	EXPECT_TRUE(std::abs(shortestPath.DistTo(0) - 0.0) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(1) - 1.05) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(2) - 0.26) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(3) - 0.99) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(4) - 0.38) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(5) - 0.73) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(6) - 1.51) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(7) - 0.60) < 1e-9);
}
