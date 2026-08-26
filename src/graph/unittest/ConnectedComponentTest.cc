#include <gtest/gtest.h>

#include "Factory.h"
#include "graph/ConnectedComponent.h"

using namespace guozi::graph;

TEST(ConnectedComponentTest, FindsUndirectedComponents)
{
	auto graph = CreateUndirectedGraph();
	auto cc = UndirectedConnectedComponent(graph);

	EXPECT_EQ(3, cc.Count());
	for (size_t vertex = 0; vertex <= 6; vertex++)
	{
		EXPECT_TRUE(cc.Connected(vertex, 0));
		EXPECT_EQ(0, cc.Id(vertex));
	}
	EXPECT_TRUE(cc.Connected(7, 8));
	EXPECT_FALSE(cc.Connected(7, 9));
	EXPECT_EQ(1, cc.Id(7));
	EXPECT_EQ(1, cc.Id(8));
	for (size_t vertex : { 9, 10, 11, 12 })
	{
		EXPECT_EQ(2, cc.Id(vertex));
	}
}

TEST(ConnectedComponentTest, HandlesEmptyAndIsolatedGraphs)
{
	UndirectedGraph<> emptyGraph;
	auto emptyComponents = UndirectedConnectedComponent(emptyGraph);
	EXPECT_EQ(0, emptyComponents.Count());

	UndirectedGraph<> graph;
	graph.AddVertex();
	graph.AddVertex();
	graph.AddVertex();
	auto components = UndirectedConnectedComponent(graph);

	EXPECT_EQ(3, components.Count());
	EXPECT_FALSE(components.Connected(0, 1));
	EXPECT_FALSE(components.Connected(1, 2));
}

TEST(ConnectedComponentTest, FindsStronglyConnectedComponents)
{
	auto graph = CreateDirectedGraph();
	StronglyConnectedComponent scc(graph);

	EXPECT_EQ(5, scc.Count());
	EXPECT_EQ(0, scc.Id(1));
	for (size_t vertex : { 0, 2, 3, 4, 5 })
	{
		EXPECT_EQ(1, scc.Id(vertex));
	}
	for (size_t vertex : { 9, 10, 11, 12 })
	{
		EXPECT_EQ(2, scc.Id(vertex));
	}
	EXPECT_EQ(3, scc.Id(6));
	EXPECT_EQ(4, scc.Id(7));
	EXPECT_EQ(4, scc.Id(8));
}
