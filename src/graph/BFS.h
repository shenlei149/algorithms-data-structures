#pragma once

#include <queue>

namespace guozi::graph
{

template<typename Graph, typename Visitor, typename VisitedContainer>
bool BFS(const Graph &graph, size_t startId, const Visitor &visitor, VisitedContainer &visited)
{
	std::queue<size_t> queue;
	queue.push(startId);
	visited[startId] = true;

	if (!visitor.OnDiscoverVertex(startId))
	{
		return false;
	}

	while (!queue.empty())
	{
		auto current = queue.front();
		queue.pop();

		for (const auto &edgeId : graph.GetOutgoingEdgeIndices(current))
		{
			if (!visitor.OnExamineEdge(edgeId))
			{
				return false;
			}

			auto neighbor = graph.GetDstVertexId(edgeId);
			if (!visited[neighbor])
			{
				visited[neighbor] = true;

				if (!visitor.OnDiscoverVertex(neighbor))
				{
					return false;
				}

				if (!visitor.OnTreeEdge(edgeId))
				{
					return false;
				}

				queue.push(neighbor);
			}
			else
			{
				if (!visitor.OnNonTreeEdge(edgeId))
				{
					return false;
				}
			}
		}

		if (!visitor.OnFinishVertex(current))
		{
			return false;
		}
	}

	return true;
}

} // namespace guozi::graph
