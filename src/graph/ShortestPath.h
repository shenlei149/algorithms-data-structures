#include <limits>
#include <vector>

#include "Path.h"
#include "basic/IndexPriorityQueue.h"

namespace guozi::graph
{

template<typename Graph, typename WeightType>
class ShortestPath
{
public:
	WeightType DistTo(size_t vertexId) const { return distTo_[vertexId]; }

	[[nodiscard]]
	bool HasPathTo(size_t vertexId) const
	{
		return distTo_[vertexId] < std::numeric_limits<WeightType>::max();
	}

	[[nodiscard]]
	PathResult GetPathTo(size_t vertexId) const
	{
		if (!HasPathTo(vertexId))
		{
			return PathResult({});
		}

		std::vector<size_t> path;
		for (size_t at = vertexId; at != std::numeric_limits<size_t>::max(); at = edgeTo_[at])
		{
			path.push_back(at);
		}

		std::reverse(path.begin(), path.end());

		return PathResult(std::move(path));
	}

protected:
	ShortestPath(const Graph &graph, size_t source)
		: distTo_(graph.VertexCount(), std::numeric_limits<WeightType>::max())
		, edgeTo_(graph.VertexCount(), std::numeric_limits<size_t>::max())
	{
		distTo_[source] = WeightType {};
	}

protected:
	std::vector<WeightType> distTo_;
	std::vector<size_t> edgeTo_;
};

template<typename Graph, typename WeightType, typename Less = std::less<WeightType>>
class DijkstraShortestPath : public ShortestPath<Graph, WeightType>
{
public:
	DijkstraShortestPath(const Graph &graph, size_t source, Less less = Less {})
		: ShortestPath<Graph, WeightType>(graph, source)
	{
		// Bring base class members into scope to avoid prefixing with this->
		using Base = ShortestPath<Graph, WeightType>;
		using Base::distTo_;
		using Base::edgeTo_;

		basic::IndexPriorityQueue<WeightType, Less> pq(graph.VertexCount(), less);
		pq.Push(source, distTo_[source]);

		while (!pq.Empty())
		{
			size_t w = pq.TopIndex();
			pq.Pop();

			// If the smallest distance is infinity, remaining vertices are unreachable
			if (distTo_[w] == std::numeric_limits<WeightType>::max())
			{
				break;
			}

			for (size_t edgeId : graph.GetOutgoingEdgeIndices(w))
			{
				size_t v = graph.GetDstVertexId(edgeId);
				WeightType weight = graph.GetEdgeWeight(edgeId);
				auto newDist = distTo_[w] + weight;
				if (less(newDist, distTo_[v]))
				{
					distTo_[v] = newDist;
					edgeTo_[v] = w;
					pq.Upsert(v, distTo_[v]);
				}
			}
		}
	}
};

} // namespace guozi::graph
