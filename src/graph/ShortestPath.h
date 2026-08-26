#pragma once

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
	[[nodiscard]] WeightType DistTo(size_t vertexId) const { return distTo_[vertexId]; }

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
	// Bring base class members into scope to avoid prefixing with this->
	using Base = ShortestPath<Graph, WeightType>;
	using Base::distTo_;
	using Base::edgeTo_;

public:
	DijkstraShortestPath(const Graph &graph, size_t source, Less less = Less {})
		: ShortestPath<Graph, WeightType>(graph, source)
	{
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

			for (size_t edgeId : graph.OutgoingEdgeIndices(w))
			{
				size_t v = graph.GetDstVertexId(edgeId);
				WeightType weight = static_cast<WeightType>(graph.GetEdge(edgeId));
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

template<typename Graph, typename WeightType, typename Less = std::less<WeightType>>
class BellmanFordShortestPath : public ShortestPath<Graph, WeightType>
{
	// Bring base class members into scope to avoid prefixing with this->
	using Base = ShortestPath<Graph, WeightType>;
	using Base::distTo_;
	using Base::edgeTo_;

public:
	BellmanFordShortestPath(const Graph &graph, size_t source, Less less = Less {})
		: ShortestPath<Graph, WeightType>(graph, source)
	{
		size_t V = graph.VertexCount();

		std::vector<std::vector<WeightType>> L(V + 1,
											   std::vector<WeightType>(V, std::numeric_limits<WeightType>::max()));
		for (size_t i = 0; i < V; i++)
		{
			L[0][i] = std::numeric_limits<WeightType>::max();
		}
		L[0][source] = WeightType {};

		for (size_t i = 1; i <= V; i++)
		{
			bool stable = true;
			for (size_t v = 0; v < V; v++)
			{
				L[i][v] = L[i - 1][v];
				for (auto edgeId : graph.IncomingEdgeIndices(v))
				{
					auto w = graph.GetSrcVertexId(edgeId);
					WeightType weight = static_cast<WeightType>(graph.GetEdge(edgeId));
					if (L[i - 1][w] != std::numeric_limits<WeightType>::max() && less(L[i - 1][w] + weight, L[i][v]))
					{
						L[i][v] = L[i - 1][w] + weight;
						edgeTo_[v] = w;
						stable = false;
					}
				}
			}

			if (stable)
			{
				distTo_ = std::move(L[i]);
				return;
			}
		}

		// there is a negative cycle
		std::fill(edgeTo_.begin(), edgeTo_.end(), std::numeric_limits<size_t>::max());

		hasNegativeCycle_ = true;
	}

	[[nodiscard]] bool HasNegativeCycle() const noexcept { return hasNegativeCycle_; }

private:
	bool hasNegativeCycle_ = false;
};

template<typename Graph, typename WeightType>
class AllPairsShortestPath
{
public:
	WeightType Dist(size_t src, size_t dst) const { return dist_[src][dst]; }

protected:
	explicit AllPairsShortestPath(const Graph &graph)
		: dist_(graph.VertexCount(),
				std::vector<WeightType>(graph.VertexCount(), std::numeric_limits<WeightType>::max()))
		, lastHop_(graph.VertexCount(), std::vector<size_t>(graph.VertexCount(), std::numeric_limits<size_t>::max()))
	{
		for (size_t v = 0; v < graph.VertexCount(); v++)
		{
			dist_[v][v] = WeightType {};

			for (size_t edgeId : graph.OutgoingEdgeIndices(v))
			{
				size_t w = graph.GetOtherVertexId(edgeId, v);
				WeightType weight = static_cast<WeightType>(graph.GetEdge(edgeId));
				dist_[v][w] = weight;

				lastHop_[v][w] = v;
			}
		}
	}

	std::vector<std::vector<WeightType>> dist_;
	std::vector<std::vector<size_t>> lastHop_;
};

template<typename Graph, typename WeightType, typename Less = std::less<WeightType>>
class FloydWarshallAllPairsShortestPath : public AllPairsShortestPath<Graph, WeightType>
{
	// Bring base class members into scope to avoid prefixing with this->
	using Base = AllPairsShortestPath<Graph, WeightType>;
	using Base::dist_;
	using Base::lastHop_;

public:
	FloydWarshallAllPairsShortestPath(const Graph &graph, Less less = Less {})
		: AllPairsShortestPath<Graph, WeightType>(graph)
	{
		size_t V = graph.VertexCount();

		for (size_t k = 0; k < V; k++)
		{
			for (size_t i = 0; i < V; i++)
			{
				for (size_t j = 0; j < V; j++)
				{
					if (dist_[i][k] != std::numeric_limits<WeightType>::max() &&
						dist_[k][j] != std::numeric_limits<WeightType>::max() &&
						less(dist_[i][k] + dist_[k][j], dist_[i][j]))
					{
						dist_[i][j] = dist_[i][k] + dist_[k][j];
						lastHop_[i][j] = lastHop_[k][j];
					}
				}
			}
		}

		for (size_t v = 0; v < V; v++)
		{
			if (less(dist_[v][v], WeightType {}))
			{
				hasNegativeCycle_ = true;
				break;
			}
		}
	}

	[[nodiscard]] bool HasNegativeCycle() const noexcept { return hasNegativeCycle_; }

private:
	bool hasNegativeCycle_ = false;
};

} // namespace guozi::graph
