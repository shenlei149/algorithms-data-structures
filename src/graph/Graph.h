#pragma once

#include <stdexcept>
#include <vector>

template<typename V, typename E>
class Graph
{
private:
	struct Vertex
	{
		V data_;
		std::vector<size_t> outgoingEdgeIndices_;

		Vertex(const V &data)
			: data_(data)
		{
		}
	};

	struct Edge
	{
		size_t src_;
		size_t dst_;
		E data_;

		Edge(size_t src, size_t dst, const E &data)
			: src_(src)
			, dst_(dst)
			, data_(data)
		{
		}
	};

public:
	Graph(bool directed = false)
		: isDirected_(directed)
	{
	}

	size_t
	AddVertex(const V &data)
	{
		vertices_.emplace_back(data);

		return vertices_.size() - 1;
	}

	size_t
	AddEdge(size_t srcId, size_t dstId, const E &data)
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

	[[nodiscard]]
	size_t
	GetVertexCount() const
	{
		return vertices_.size();
	}

	[[nodiscard]]
	size_t
	GetEdgeCount() const
	{
		return edges_.size();
	}

	[[nodiscard]]
	const V &
	GetVertex(size_t vertexId) const
	{
		return vertices_[vertexId].data_;
	}

	[[nodiscard]]
	const std::vector<size_t> &
	GetOutgoingEdges(size_t vertexId) const
	{
		return vertices_[vertexId].outgoingEdgeIndices_;
	}

	[[nodiscard]]
	const E &
	GetEdge(size_t edgeId) const
	{
		return edges_[edgeId].data_;
	}

private:
	std::vector<Vertex> vertices_;
	std::vector<Edge> edges_;
	bool isDirected_;
};
