#pragma once

#include <algorithm>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace guozi::tree
{

template<typename TChar, typename TWeight>
class HuffmanTree
{
private:
	struct Node
	{
		TChar data_;
		TWeight weight_;
		std::unique_ptr<Node> left_ { nullptr };
		std::unique_ptr<Node> right_ { nullptr };

		Node(TChar data, TWeight weight)
			: data_(data)
			, weight_(weight)
		{}
	};

public:
	HuffmanTree(const std::unordered_map<TChar, TWeight> &charWeights)
	{
		if (charWeights.empty())
		{
			return;
		}

		std::vector<std::unique_ptr<Node>> leaves;
		for (const auto &[c, weight] : charWeights)
		{
			leaves.push_back(std::make_unique<Node>(c, weight));
		}

		std::sort(leaves.begin(),
				  leaves.end(),
				  [](const std::unique_ptr<Node> &a, const std::unique_ptr<Node> &b)
				  { return a->weight_ < b->weight_; });

		std::queue<std::unique_ptr<Node>> qLeaf;
		std::queue<std::unique_ptr<Node>> qInternal;
		for (auto &leaf : leaves)
		{
			qLeaf.push(std::move(leaf));
		}

		while (qLeaf.size() + qInternal.size() > 1)
		{
			auto left = FetchMin(qLeaf, qInternal);
			auto right = FetchMin(qLeaf, qInternal);

			auto parent = std::make_unique<Node>(TChar(), left->weight_ + right->weight_);
			parent->left_ = std::move(left);
			parent->right_ = std::move(right);
			qInternal.push(std::move(parent));
		}

		root_ = FetchMin(qLeaf, qInternal);

		TWeight totalWeightWithLength = 0;
		TWeight totalWeight = 0;
		std::vector<bool> path;
		BuildEncodingTable(root_.get(), path, totalWeightWithLength, totalWeight);

		if (totalWeight > 0)
		{
			averageCodeLength_ = static_cast<double>(totalWeightWithLength) / totalWeight;
		}
	}

	const std::unordered_map<TChar, std::vector<bool>> &GetEncodingTable() const { return encodingTable_; }

	double GetAverageCodeLength() const { return averageCodeLength_; }

private:
	void BuildEncodingTable(Node *node, std::vector<bool> &path, TWeight &totalWeightWithLength, TWeight &totalWeight)
	{
		if (!node)
		{
			return;
		}

		if (!node->left_ && !node->right_)
		{
			encodingTable_[node->data_] = path;

			totalWeightWithLength += node->weight_ * path.size();
			totalWeight += node->weight_;

			return;
		}

		if (node->left_)
		{
			path.push_back(false);
			BuildEncodingTable(node->left_.get(), path, totalWeightWithLength, totalWeight);
			path.pop_back();
		}

		if (node->right_)
		{
			path.push_back(true);
			BuildEncodingTable(node->right_.get(), path, totalWeightWithLength, totalWeight);
			path.pop_back();
		}
	}

	std::unique_ptr<Node> FetchMin(std::queue<std::unique_ptr<Node>> &q1, std::queue<std::unique_ptr<Node>> &q2)
	{
		if (q1.empty())
		{
			auto node = std::move(q2.front());
			q2.pop();
			return node;
		}

		if (q2.empty())
		{
			auto node = std::move(q1.front());
			q1.pop();
			return node;
		}

		if (q1.front()->weight_ < q2.front()->weight_)
		{
			auto node = std::move(q1.front());
			q1.pop();
			return node;
		}
		else
		{
			auto node = std::move(q2.front());
			q2.pop();
			return node;
		}
	}

private:
	std::unique_ptr<Node> root_ { nullptr };
	std::unordered_map<TChar, std::vector<bool>> encodingTable_ {};
	double averageCodeLength_ { 0.0 };
};

} // namespace guozi::tree
