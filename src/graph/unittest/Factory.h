#pragma once

#include "graph/Graph.h"

using namespace guozi::graph;

UndirectedGraph<> CreateUndirectedGraph()
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

DirectedGraph<> CreateDirectedGraph()
{
	DirectedGraph<> graph;
	for (size_t i = 0; i < 13; i++)
	{
		graph.AddVertex();
	}

	graph.AddEdge(4, 2);
	graph.AddEdge(2, 3);
	graph.AddEdge(3, 2);
	graph.AddEdge(6, 0);
	graph.AddEdge(0, 1);
	graph.AddEdge(2, 0);
	graph.AddEdge(11, 12);
	graph.AddEdge(12, 9);
	graph.AddEdge(9, 10);
	graph.AddEdge(9, 11);
	graph.AddEdge(8, 9);
	graph.AddEdge(10, 12);
	graph.AddEdge(11, 4);
	graph.AddEdge(4, 3);
	graph.AddEdge(3, 5);
	graph.AddEdge(7, 8);
	graph.AddEdge(8, 7);
	graph.AddEdge(5, 4);
	graph.AddEdge(0, 5);
	graph.AddEdge(6, 4);
	graph.AddEdge(6, 9);
	graph.AddEdge(7, 6);
	return graph;
}

DirectedGraph<std::monostate, double> CreateWeightedDirectedGraph()
{
	DirectedGraph<std::monostate, double> graph;
	for (size_t i = 0; i < 8; i++)
	{
		graph.AddVertex();
	}

	graph.AddEdge(4, 5, 0.35);
	graph.AddEdge(5, 4, 0.35);
	graph.AddEdge(4, 7, 0.37);
	graph.AddEdge(5, 7, 0.28);
	graph.AddEdge(7, 5, 0.28);
	graph.AddEdge(5, 1, 0.32);
	graph.AddEdge(0, 4, 0.38);
	graph.AddEdge(0, 2, 0.26);
	graph.AddEdge(7, 3, 0.39);
	graph.AddEdge(1, 3, 0.29);
	graph.AddEdge(2, 7, 0.34);
	graph.AddEdge(6, 2, 0.40);
	graph.AddEdge(3, 6, 0.52);
	graph.AddEdge(6, 0, 0.58);
	graph.AddEdge(6, 4, 0.93);

	return graph;
}
