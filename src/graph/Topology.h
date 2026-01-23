#pragma once

#include <algorithm>
#include <vector>

namespace guozi::graph
{

template<typename Graph>
class Topology
{
	struct Visitor
	{
		Visitor(Topology &topology)
			: topology_(topology)
		{}

		bool OnTreeEdge(size_t) const { return true; }

		bool OnNonTreeEdge(size_t) const { return true; }

		bool OnDiscoverVertex(size_t) const { return true; }

		bool OnExamineEdge(size_t) const { return true; }

		bool OnFinishVertex(size_t vertexId)
		{
			topology_.order_.push_back(vertexId);
			return true;
		}

		Topology &topology_;
	};

public:
	Topology(const Graph &graph)
	{
		std::vector<bool> visited(graph.VertexCount(), false);
		Visitor visitor(*this);

		for (size_t i = 0; i < graph.VertexCount(); i++)
		{
			if (!visited[i])
			{
				BFS(graph, i, visitor, visited);
			}
		}

		std::reverse(order_.begin(), order_.end());
	}

	[[nodiscard]] const std::span<size_t> &Order() const { return order_; }

private:
	std::vector<size_t> order__;
};

} // namespace guozi::graph
