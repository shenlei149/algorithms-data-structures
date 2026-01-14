#pragma once

#include <stdexcept>
#include <vector>

namespace guozi::graph
{

template<typename V = std::monostate, typename E = std::monostate>
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
	Graph(bool directed = false)
		: isDirected_(directed)
	{}

	size_t AddVertex(const V &data = {})
	{
		vertices_.emplace_back(data);

		return vertices_.size() - 1;
	}

	size_t AddEdge(size_t srcId, size_t dstId, const E &data = {})
	{
		if (srcId >= vertices_.size() || dstId >= vertices_.size())
		{
			throw std::out_of_range("Vertex index out of range");
		}

		edges_.emplace_back(srcId, dstId, data);

		size_t newEdgeIdx = edges_.size() - 1;
		vertices_[srcId].outgoingEdgeIndices_.push_back(newEdgeIdx);
		if (!isDirected_)
		{
			vertices_[dstId].outgoingEdgeIndices_.push_back(newEdgeIdx);
		}

		return newEdgeIdx;
	}

	[[nodiscard]] size_t GetVertexCount() const { return vertices_.size(); }

	[[nodiscard]] size_t GetEdgeCount() const { return edges_.size(); }

	[[nodiscard]]
	const std::vector<size_t> &GetOutgoingEdgeIndices(size_t vertexId) const
	{
		return vertices_[vertexId].outgoingEdgeIndices_;
	}

	[[nodiscard]] size_t GetSrcVertexId(size_t edgeId) const { return edges_[edgeId].src_; }

	[[nodiscard]] size_t GetDstVertexId(size_t edgeId) const { return edges_[edgeId].dst_; }

	[[nodiscard]] const V &GetVertex(size_t vertexId) const { return vertices_[vertexId].data_; }

	[[nodiscard]] const E &GetEdge(size_t edgeId) const { return edges_[edgeId].data_; }

private:
	std::vector<Vertex> vertices_;
	std::vector<Edge> edges_;
	bool isDirected_;
};

} // namespace guozi::graph
