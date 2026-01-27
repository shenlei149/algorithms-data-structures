#pragma once

#include <stdexcept>
#include <vector>

namespace guozi::graph
{

template<bool Directed, typename V = std::monostate, typename E = std::monostate>
class Graph
{
public:
	using VertexType = V;
	using EdgeType = E;

private:
	struct Vertex
	{
		[[no_unique_address]] V data_;
		std::vector<size_t> outgoingEdgeIndices_;

		Vertex(const V &data)
			: data_(data)
		{}
	};

	struct Edge
	{
		size_t src_;
		size_t dst_;

		[[no_unique_address]] E data_;

		Edge(size_t src, size_t dst, const E &data)
			: src_(src)
			, dst_(dst)
			, data_(data)
		{}
	};

public:
	size_t AddVertex(const V data = {})
	{
		vertices_.emplace_back(std::move(data));

		return vertices_.size() - 1;
	}

	size_t AddEdge(size_t srcId, size_t dstId, const E data = {})
	{
		if (srcId >= vertices_.size() || dstId >= vertices_.size())
		{
			throw std::out_of_range("Vertex index out of range");
		}

		edges_.emplace_back(srcId, dstId, std::move(data));

		size_t newEdgeIdx = edges_.size() - 1;
		vertices_[srcId].outgoingEdgeIndices_.push_back(newEdgeIdx);
		if constexpr (!Directed)
		{
			vertices_[dstId].outgoingEdgeIndices_.push_back(newEdgeIdx);
		}

		return newEdgeIdx;
	}

	[[nodiscard]] size_t VertexCount() const { return vertices_.size(); }

	[[nodiscard]] size_t EdgeCount() const { return edges_.size(); }

	[[nodiscard]]
	const std::vector<size_t> &OutgoingEdgeIndices(size_t vertexId) const
	{
		return vertices_[vertexId].outgoingEdgeIndices_;
	}

	[[nodiscard]] size_t GetSrcVertexId(size_t edgeId) const { return edges_[edgeId].src_; }

	[[nodiscard]] size_t GetDstVertexId(size_t edgeId) const { return edges_[edgeId].dst_; }

	[[nodiscard]] const V &GetVertex(size_t vertexId) const { return vertices_[vertexId].data_; }

	[[nodiscard]] const E &GetEdge(size_t edgeId) const { return edges_[edgeId].data_; }

	Graph<Directed, V, E> Transpose() const
	{
		Graph<Directed, V, E> reverseGraph;
		reverseGraph.vertices_.reserve(vertices_.size());
		reverseGraph.edges_.reserve(edges_.size());

		for (size_t i = 0; i < vertices_.size(); i++)
		{
			reverseGraph.AddVertex(vertices_[i].data_);
		}

		for (size_t i = 0; i < edges_.size(); i++)
		{
			reverseGraph.AddEdge(edges_[i].dst_, edges_[i].src_, edges_[i].data_);
		}

		return reverseGraph;
	}

private:
	std::vector<Vertex> vertices_;
	std::vector<Edge> edges_;
};

template<typename V = std::monostate, typename E = std::monostate>
using UndirectedGraph = Graph<false, V, E>;

template<typename V = std::monostate, typename E = std::monostate>
using DirectedGraph = Graph<true, V, E>;

} // namespace guozi::graph
