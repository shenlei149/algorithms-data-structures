#pragma once

#include <algorithm>
#include <cstdint>
#include <span>
#include <vector>

#include "Traversals.h"

namespace guozi::graph
{

template<typename Graph>
class Path;

class PathResult
{
public:
	[[nodiscard]] std::span<const size_t> GetPath() const { return path_; }

	using const_iterator = std::vector<size_t>::const_iterator;

	const_iterator begin() const { return path_.begin(); }

	const_iterator end() const { return path_.end(); }

	bool empty() const { return path_.empty(); }

	size_t size() const { return path_.size(); }

	operator std::vector<size_t>() const { return path_; }

	PathResult(std::vector<size_t> path)
		: path_(std::move(path))
	{}

private:
	std::vector<size_t> path_;
};

template<typename Graph>
class Path
{
	struct Visitor
	{
		Visitor(const Graph &graph, Path &path)
			: graph_(graph)
			, path_(path)
		{}

		bool OnTreeEdge(size_t edgeId) const
		{
			path_.edgeTo_[graph_.GetDstVertexId(edgeId)] = graph_.GetSrcVertexId(edgeId);

			return true;
		}

		bool OnNonTreeEdge(size_t) const { return true; }

		bool OnDiscoverVertex(size_t) const { return true; }

		bool OnExamineEdge(size_t) const { return true; }

		bool OnFinishVertex(size_t) const { return true; }

	private:
		const Graph &graph_;
		Path &path_;
	};

public:
	Path(const Graph &graph, size_t srcId)
		: srcId_(srcId)
		, edgeTo_(graph.VertexCount(), -1)
		, visited_(graph.VertexCount(), false)
	{
		BFS(graph, srcId, Visitor(graph, *this), visited_);
	}

	[[nodiscard]] bool HasPathTo(size_t dstId) const { return visited_[dstId] != 0; }

	[[nodiscard]]
	PathResult GetPathTo(size_t dstId) const
	{
		if (!HasPathTo(dstId))
		{
			return PathResult({});
		}

		std::vector<size_t> path;
		for (size_t current = dstId; current != srcId_; current = edgeTo_[current])
		{
			path.push_back(current);
		}
		path.push_back(srcId_);

		std::reverse(path.begin(), path.end());

		return PathResult(std::move(path));
	}

private:
	size_t srcId_;
	std::vector<size_t> edgeTo_;
	std::vector<uint8_t> visited_;
};

} // namespace guozi::graph
