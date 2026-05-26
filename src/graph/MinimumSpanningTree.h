#pragma once

#include <cstdint>
#include <map>
#include <numeric>
#include <vector>

#include "Graph.h"
#include "basic/Heap.h"
#include "basic/UnionFind.h"

namespace guozi::graph
{

template<typename Graph, typename WeightType>
class MinimumSpanningTree
{
public:
	const std::vector<size_t> &EdgeIds() const { return edges_; }

	const WeightType Weight() const { return weight_; }

protected:
	std::vector<size_t> edges_;
	WeightType weight_ = {};
};

template<typename Graph, typename WeightType, typename Less = std::less<WeightType>>
class PrimMinimumSpanningTree : public MinimumSpanningTree<Graph, WeightType>
{
	// Bring base class members into scope to avoid prefixing with this->
	using Base = MinimumSpanningTree<Graph, WeightType>;

private:
	struct HeapElement
	{
		size_t vertexId_;
		WeightType weight_;
	};

public:
	PrimMinimumSpanningTree(const Graph &graph, Less less = Less {})
		: Base()
	{
		Base::edges_.reserve(graph.VertexCount() - 1);

		std::vector<uint8_t> visited(graph.VertexCount(), 0);
		std::vector<size_t> winner(graph.VertexCount());
		std::vector<WeightType> key(graph.VertexCount(), std::numeric_limits<WeightType>::max());

		auto cmp = [&less](const HeapElement &a, const HeapElement &b) { return less(a.weight_, b.weight_); };
		basic::Heap<HeapElement, decltype(cmp)> minHeap(cmp);

		// visit the first vertex & initialize the heap and key/winner arrays
		visited[0] = 1;
		auto &edges = graph.OutgoingEdgeIndices(0);
		std::map<size_t, size_t> vertexToEdgeId;
		for (auto edgeId : edges)
		{
			size_t dstId = graph.GetOtherVertexId(edgeId, 0);
			vertexToEdgeId[dstId] = edgeId;
		}
		for (size_t i = 1; i < graph.VertexCount(); i++)
		{
			if (vertexToEdgeId.contains(i))
			{
				key[i] = static_cast<WeightType>(graph.GetEdge(vertexToEdgeId[i]));
				winner[i] = vertexToEdgeId[i];
			}
			else
			{
				key[i] = std::numeric_limits<WeightType>::max();
				winner[i] = {};
			}

			minHeap.Push({ i, key[i] });
		}

		// suppose the graph is connected, we will visit all vertices and add |V| - 1 edges to the MST
		while (!minHeap.Empty())
		{
			auto [vertexId, weight] = minHeap.Top();
			minHeap.Pop();

			// lazy deletion: if the vertex is already visited, it means we have found a better edge to connect it to
			// the MST, so we can skip it
			if (visited[vertexId])
			{
				continue;
			}

			visited[vertexId] = 1;
			Base::edges_.push_back(winner[vertexId]);
			Base::weight_ += weight;

			for (auto edgeId : graph.OutgoingEdgeIndices(vertexId))
			{
				size_t dstId = graph.GetOtherVertexId(edgeId, vertexId);
				if (visited[dstId] == 0)
				{
					auto edgeWeight = static_cast<WeightType>(graph.GetEdge(edgeId));
					if (less(edgeWeight, key[dstId]))
					{
						key[dstId] = edgeWeight;
						winner[dstId] = edgeId;
						minHeap.Push({ dstId, key[dstId] });
					}
				}
			}
		}
	}
};

template<typename Graph, typename WeightType, typename Less = std::less<WeightType>>
class KruskalMinimumSpanningTree : public MinimumSpanningTree<Graph, WeightType>
{
	// Bring base class members into scope to avoid prefixing with this->
	using Base = MinimumSpanningTree<Graph, WeightType>;

public:
	KruskalMinimumSpanningTree(const Graph &graph, Less less = Less {})
		: Base()
	{
		Base::edges_.reserve(graph.VertexCount() - 1);

		std::vector<size_t> edgeIds(graph.EdgeCount());
		std::iota(edgeIds.begin(), edgeIds.end(), 0);

		std::sort(
			edgeIds.begin(),
			edgeIds.end(),
			[&graph, &less](size_t a, size_t b)
			{ return less(static_cast<WeightType>(graph.GetEdge(a)), static_cast<WeightType>(graph.GetEdge(b))); });

		basic::UnionFind uf(graph.VertexCount());
		for (auto edgeId : edgeIds)
		{
			size_t srcId = graph.GetSrcVertexId(edgeId);
			size_t dstId = graph.GetDstVertexId(edgeId);
			if (!uf.Connected(srcId, dstId))
			{
				uf.Union(srcId, dstId);
				Base::edges_.push_back(edgeId);
				Base::weight_ += static_cast<WeightType>(graph.GetEdge(edgeId));
			}
		}
	}
};

} // namespace guozi::graph
