#pragma once

#include <cstdint>
#include <vector>

#include "BFS.h"
#include "Graph.h"

namespace guozi::graph
{

template<typename Graph>
class UndirectedConnectedComponent
{
	struct Visitor
	{
		Visitor(UndirectedConnectedComponent &cc)
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
		UndirectedConnectedComponent &cc_;
	};

public:
	UndirectedConnectedComponent(const Graph &graph)
		: ccIds_(graph.GetVertexCount())
		, visited_(graph.GetVertexCount(), false)
	{
		for (size_t i = 0; i < graph.GetVertexCount(); ++i)
		{
			if (visited_[i] == 0)
			{
				BFS(graph, i, Visitor(*this), visited_);

				ccCount_++;
			}
		}
	}

	[[nodiscard]] size_t Count() const { return ccCount_; }

	[[nodiscard]] size_t Id(size_t vertexId) const { return ccIds_[vertexId]; }

	[[nodiscard]] bool Connected(size_t srcId, size_t dstId) const { return ccIds_[srcId] == ccIds_[dstId]; }

private:
	std::vector<size_t> ccIds_;
	std::vector<uint8_t> visited_;
	size_t ccCount_ = 0;
};

} // namespace guozi::graph
