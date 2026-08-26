#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include "Factory.h"
#include "graph/ShortestPath.h"

using namespace guozi::graph;

TEST(ShortestPathTest, DijkstraFindsShortestPaths)
{
	auto graph = CreateWeightedDirectedGraph();
	auto shortestPath = DijkstraShortestPath<decltype(graph), double>(graph, 0);

	const std::vector<double> expected { 0.0, 1.05, 0.26, 0.99, 0.38, 0.73, 1.51, 0.60 };
	for (size_t vertex = 0; vertex < expected.size(); vertex++)
	{
		EXPECT_TRUE(std::abs(shortestPath.DistTo(vertex) - expected[vertex]) < 1e-9);
		EXPECT_TRUE(shortestPath.HasPathTo(vertex));
	}
}

TEST(ShortestPathTest, FloydWarshallFindsAllPairsShortestPaths)
{
	DirectedGraph<std::monostate, int> graph;
	for (size_t i = 0; i < 5; i++)
	{
		graph.AddVertex();
	}

	graph.AddEdge(0, 1, 3);
	graph.AddEdge(0, 2, 8);
	graph.AddEdge(0, 4, -4);
	graph.AddEdge(1, 3, 1);
	graph.AddEdge(1, 4, 7);
	graph.AddEdge(2, 1, 4);
	graph.AddEdge(3, 0, 2);
	graph.AddEdge(3, 2, -5);
	graph.AddEdge(4, 3, 6);

	auto shortestPath = FloydWarshallAllPairsShortestPath<decltype(graph), int>(graph);
	const std::vector<std::vector<int>> expected {
		{ 0, 1, -3, 2, -4 },
		{ 3, 0, -4, 1, -1 },
		{ 7, 4, 0, 5, 3 },
		{ 2, -1, -5, 0, -2 },
		{ 8, 5, 1, 6, 0 },
	};

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	for (size_t src = 0; src < expected.size(); src++)
	{
		for (size_t dst = 0; dst < expected[src].size(); dst++)
		{
			EXPECT_EQ(expected[src][dst], shortestPath.Dist(src, dst));
		}
	}
}

TEST(ShortestPathTest, FloydWarshallHandlesUnreachableAndZeroWeightEdges)
{
	DirectedGraph<std::monostate, int> graph;
	for (size_t i = 0; i < 5; i++)
	{
		graph.AddVertex();
	}
	graph.AddEdge(0, 1, 0);
	graph.AddEdge(1, 2, 0);
	graph.AddEdge(0, 2, 4);

	auto shortestPath = FloydWarshallAllPairsShortestPath<decltype(graph), int>(graph);
	const auto infinity = std::numeric_limits<int>::max();

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_EQ(0, shortestPath.Dist(0, 0));
	EXPECT_EQ(0, shortestPath.Dist(0, 1));
	EXPECT_EQ(0, shortestPath.Dist(0, 2));
	EXPECT_EQ(infinity, shortestPath.Dist(0, 3));
	EXPECT_EQ(infinity, shortestPath.Dist(4, 0));
	EXPECT_EQ(0, shortestPath.Dist(4, 4));
}

TEST(ShortestPathTest, FloydWarshallHandlesSingleVertexGraph)
{
	DirectedGraph<std::monostate, int> graph;
	auto vertex = graph.AddVertex();
	auto shortestPath = FloydWarshallAllPairsShortestPath<decltype(graph), int>(graph);

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_EQ(0, shortestPath.Dist(vertex, vertex));
}

TEST(ShortestPathTest, FloydWarshallSupportsUndirectedFloatingPointGraphs)
{
	UndirectedGraph<std::monostate, double> graph;
	auto a = graph.AddVertex();
	auto b = graph.AddVertex();
	auto c = graph.AddVertex();

	graph.AddEdge(a, b, 1.5);
	graph.AddEdge(b, c, 2.25);
	graph.AddEdge(a, c, 5.0);

	auto shortestPath = FloydWarshallAllPairsShortestPath<decltype(graph), double>(graph);

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_TRUE(std::abs(shortestPath.Dist(a, a) - 0.0) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.Dist(a, b) - 1.5) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.Dist(a, c) - 3.75) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.Dist(c, a) - 3.75) < 1e-9);
}

TEST(ShortestPathTest, FloydWarshallDetectsNegativeCycles)
{
	DirectedGraph<std::monostate, int> graph;
	auto source = graph.AddVertex();
	auto first = graph.AddVertex();
	auto second = graph.AddVertex();

	graph.AddEdge(source, first, 4);
	graph.AddEdge(first, second, -3);
	graph.AddEdge(second, first, -2);

	auto shortestPath = FloydWarshallAllPairsShortestPath<decltype(graph), int>(graph);

	EXPECT_TRUE(shortestPath.HasNegativeCycle());
	EXPECT_LT(shortestPath.Dist(first, first), 0);
	EXPECT_LT(shortestPath.Dist(second, second), 0);
}

TEST(ShortestPathTest, FloydWarshallDetectsNegativeCyclesInDisconnectedComponents)
{
	DirectedGraph<std::monostate, int> graph;
	auto source = graph.AddVertex();
	auto target = graph.AddVertex();
	auto cycleFirst = graph.AddVertex();
	auto cycleSecond = graph.AddVertex();

	graph.AddEdge(source, target, 7);
	graph.AddEdge(cycleFirst, cycleSecond, -1);
	graph.AddEdge(cycleSecond, cycleFirst, -1);

	auto shortestPath = FloydWarshallAllPairsShortestPath<decltype(graph), int>(graph);

	EXPECT_TRUE(shortestPath.HasNegativeCycle());
	EXPECT_EQ(7, shortestPath.Dist(source, target));
	EXPECT_EQ(0, shortestPath.Dist(source, source));
}

TEST(ShortestPathTest, BellmanFordFindsShortestPathsWithNegativeEdges)
{
	DirectedGraph<std::string, int> graph;
	auto s = graph.AddVertex("s");
	auto u = graph.AddVertex("u");
	auto v = graph.AddVertex("v");
	auto t = graph.AddVertex("t");
	auto w = graph.AddVertex("w");

	graph.AddEdge(s, v, 4);
	graph.AddEdge(s, u, 2);
	graph.AddEdge(u, v, -1);
	graph.AddEdge(v, t, 4);
	graph.AddEdge(u, w, 2);
	graph.AddEdge(w, t, 2);

	auto shortestPath = BellmanFordShortestPath<decltype(graph), int>(graph, s);

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_EQ(0, shortestPath.DistTo(s));
	EXPECT_EQ(2, shortestPath.DistTo(u));
	EXPECT_EQ(1, shortestPath.DistTo(v));
	EXPECT_EQ(5, shortestPath.DistTo(t));
	EXPECT_EQ(4, shortestPath.DistTo(w));
	EXPECT_EQ((std::vector<size_t> { s, u, v, t }), static_cast<std::vector<size_t>>(shortestPath.GetPathTo(t)));
}

TEST(ShortestPathTest, BellmanFordHandlesUnreachableVerticesAndNegativeCycles)
{
	DirectedGraph<std::monostate, int> graph;
	auto source = graph.AddVertex();
	auto target = graph.AddVertex();
	auto cycleFirst = graph.AddVertex();
	auto cycleSecond = graph.AddVertex();

	graph.AddEdge(source, target, 5);
	graph.AddEdge(cycleFirst, cycleSecond, -1);
	graph.AddEdge(cycleSecond, cycleFirst, -1);

	auto shortestPath = BellmanFordShortestPath<decltype(graph), int>(graph, source);

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_EQ(0, shortestPath.DistTo(source));
	EXPECT_EQ(5, shortestPath.DistTo(target));
	EXPECT_FALSE(shortestPath.HasPathTo(cycleFirst));
	EXPECT_FALSE(shortestPath.HasPathTo(cycleSecond));
	EXPECT_TRUE(shortestPath.GetPathTo(cycleFirst).empty());
}

TEST(ShortestPathTest, BellmanFordHandlesZeroWeightEdgesAndMultipleRelaxations)
{
	DirectedGraph<std::monostate, int> graph;
	auto source = graph.AddVertex();
	auto first = graph.AddVertex();
	auto second = graph.AddVertex();
	auto target = graph.AddVertex();

	graph.AddEdge(source, first, 8);
	graph.AddEdge(first, second, -3);
	graph.AddEdge(second, target, -2);
	graph.AddEdge(source, target, 10);

	auto shortestPath = BellmanFordShortestPath<decltype(graph), int>(graph, source);

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_EQ(0, shortestPath.DistTo(source));
	EXPECT_EQ(8, shortestPath.DistTo(first));
	EXPECT_EQ(5, shortestPath.DistTo(second));
	EXPECT_EQ(3, shortestPath.DistTo(target));
	EXPECT_EQ((std::vector<size_t> { source, first, second, target }),
			  static_cast<std::vector<size_t>>(shortestPath.GetPathTo(target)));
}

TEST(ShortestPathTest, BellmanFordDetectsReachableNegativeCycle)
{
	DirectedGraph<std::monostate, int> graph;
	auto source = graph.AddVertex();
	auto first = graph.AddVertex();
	auto second = graph.AddVertex();

	graph.AddEdge(source, first, 1);
	graph.AddEdge(first, second, -2);
	graph.AddEdge(second, first, -2);

	auto shortestPath = BellmanFordShortestPath<decltype(graph), int>(graph, source);

	EXPECT_TRUE(shortestPath.HasNegativeCycle());
	EXPECT_TRUE(shortestPath.HasPathTo(source));
	EXPECT_FALSE(shortestPath.HasPathTo(first));
	EXPECT_FALSE(shortestPath.HasPathTo(second));
	EXPECT_EQ((std::vector<size_t> { source }), static_cast<std::vector<size_t>>(shortestPath.GetPathTo(source)));
	EXPECT_TRUE(shortestPath.GetPathTo(first).empty());
}

TEST(ShortestPathTest, BellmanFordSupportsFloatingPointWeights)
{
	DirectedGraph<std::monostate, double> graph;
	auto source = graph.AddVertex();
	auto via = graph.AddVertex();
	auto target = graph.AddVertex();

	graph.AddEdge(source, target, 1.5);
	graph.AddEdge(source, via, 0.75);
	graph.AddEdge(via, target, -0.25);

	auto shortestPath = BellmanFordShortestPath<decltype(graph), double>(graph, source);

	EXPECT_FALSE(shortestPath.HasNegativeCycle());
	EXPECT_TRUE(std::abs(shortestPath.DistTo(source) - 0.0) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(via) - 0.75) < 1e-9);
	EXPECT_TRUE(std::abs(shortestPath.DistTo(target) - 0.5) < 1e-9);
	EXPECT_EQ((std::vector<size_t> { source, via, target }),
			  static_cast<std::vector<size_t>>(shortestPath.GetPathTo(target)));
}
