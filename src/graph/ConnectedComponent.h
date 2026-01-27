#pragma once

#include <cstdint>
#include <vector>

#include "Graph.h"
#include "Topology.h"
#include "Traversals.h"

namespace guozi::graph
{

template<typename Graph, bool Strongly>
class ConnectedComponent
{
	struct Visitor
	{
		Visitor(ConnectedComponent &cc)
			: cc_(cc)
		{}

		bool OnTreeEdge(size_t) const { return true; }

		bool OnNonTreeEdge(size_t) const { return true; }

		bool OnDiscoverVertex(size_t vertexId) const
		{
			cc_.ccIds_[vertexId] = cc_.ccCount_;
			return true;
		}

		bool OnExamineEdge(size_t) const { return true; }

		bool OnFinishVertex(size_t) const { return true; }

	private:
		ConnectedComponent &cc_;
	};

public:
	ConnectedComponent(const Graph &graph)
		: ccIds_(graph.VertexCount())
	{
		std::vector<uint8_t> visited(graph.VertexCount(), false);

		if constexpr (Strongly)
		{
			// Kosaraju's algorithm
			auto reverseGraph = graph.Transpose();
			Topology topo(reverseGraph);
			for (size_t i : topo.Order())
			{
				if (visited[i] == 0)
				{
					BFS(graph, i, Visitor(*this), visited);

					ccCount_++;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < graph.VertexCount(); ++i)
			{
				if (visited[i] == 0)
				{
					BFS(graph, i, Visitor(*this), visited);

					ccCount_++;
				}
			}
		}
	}

	[[nodiscard]] size_t Count() const { return ccCount_; }

	[[nodiscard]] size_t Id(size_t vertexId) const { return ccIds_[vertexId]; }

	[[nodiscard]] bool Connected(size_t srcId, size_t dstId) const { return ccIds_[srcId] == ccIds_[dstId]; }

private:
	std::vector<size_t> ccIds_;
	size_t ccCount_ = 0;
};

template<typename Graph>
using StronglyConnectedComponent = ConnectedComponent<Graph, true>;

template<typename Graph>
using UndirectedConnectedComponent = ConnectedComponent<Graph, false>;

} // namespace guozi::graph
