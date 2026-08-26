#include <gtest/gtest.h>

#include "Factory.h"
#include "graph/Path.h"

using namespace guozi::graph;

TEST(PathTest, FindsPathsWithinConnectedComponents)
{
	auto graph = CreateUndirectedGraph();
	Path path(graph, 0);

	for (size_t vertex : { 0, 1, 2, 3, 4, 5, 6 })
	{
		EXPECT_TRUE(path.HasPathTo(vertex));
	}
	for (size_t vertex : { 7, 8, 9, 10, 11, 12 })
	{
		EXPECT_FALSE(path.HasPathTo(vertex));
		EXPECT_TRUE(path.GetPathTo(vertex).empty());
	}

	EXPECT_EQ((std::vector<size_t> { 0 }), static_cast<std::vector<size_t>>(path.GetPathTo(0)));
	EXPECT_EQ((std::vector<size_t> { 0, 1 }), static_cast<std::vector<size_t>>(path.GetPathTo(1)));
	EXPECT_EQ((std::vector<size_t> { 0, 2 }), static_cast<std::vector<size_t>>(path.GetPathTo(2)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 3 }), static_cast<std::vector<size_t>>(path.GetPathTo(3)));
	EXPECT_EQ((std::vector<size_t> { 0, 5, 4 }), static_cast<std::vector<size_t>>(path.GetPathTo(4)));
	EXPECT_EQ((std::vector<size_t> { 0, 5 }), static_cast<std::vector<size_t>>(path.GetPathTo(5)));
}

TEST(PathTest, FindsPathsFromAnotherComponent)
{
	auto graph = CreateUndirectedGraph();
	Path path(graph, 9);

	EXPECT_EQ((std::vector<size_t> { 9 }), static_cast<std::vector<size_t>>(path.GetPathTo(9)));
	EXPECT_EQ((std::vector<size_t> { 9, 10 }), static_cast<std::vector<size_t>>(path.GetPathTo(10)));
	EXPECT_EQ((std::vector<size_t> { 9, 11 }), static_cast<std::vector<size_t>>(path.GetPathTo(11)));
	EXPECT_EQ((std::vector<size_t> { 9, 12 }), static_cast<std::vector<size_t>>(path.GetPathTo(12)));

	for (size_t vertex : { 0, 1, 2, 3, 4, 5, 6, 7, 8 })
	{
		EXPECT_FALSE(path.HasPathTo(vertex));
	}
}
