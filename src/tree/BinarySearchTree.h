#pragma once

#include <functional>
#include <memory>
#include <utility>

namespace guozi::tree
{

template<typename T, typename Compare = std::less<T>>
class BinarySearchTree
{
private:
	struct Node
	{
		T data_;
		std::unique_ptr<Node> left_ { nullptr };
		std::unique_ptr<Node> right_ { nullptr };
		Node *parent_;
		int size_ { 1 }; // size of the subtree rooted at this node

		Node(const T &data, Node *parent = nullptr)
			: data_(data)
			, parent_(parent)
		{}

		template<typename Func>
		static void InOrderTraversal(Node *node, Func &&callback)
		{
			if (node)
			{
				InOrderTraversal(node->left_.get(), std::forward<Func>(callback));
				callback(node->data_);
				InOrderTraversal(node->right_.get(), std::forward<Func>(callback));
			}
		}
	};

public:
	class Iterator
	{
		friend class BinarySearchTree;

	public:
		const T &operator*() const { return current_->data_; }

		const T *operator->() const { return &current_->data_; }

		bool operator==(const Iterator &other) const { return current_ == other.current_; }

		bool operator!=(const Iterator &other) const { return current_ != other.current_; }

		Iterator &operator++()
		{
			if (current_)
			{
				current_ = BinarySearchTree::GetSuccessorNode(current_);
			}

			return *this;
		}

		Iterator &operator--()
		{
			if (current_)
			{
				current_ = BinarySearchTree::GetPredecessorNode(current_);
			}

			return *this;
		}

	private:
		explicit Iterator(Node *node)
			: current_(node)
		{}

	private:
		Node *current_;
	};

public:
	BinarySearchTree(Compare compare = Compare {})
		: root_(nullptr)
		, compare_(compare)
	{}

	Iterator Find(const T &value) const
	{
		Node *current = root_.get();
		while (current)
		{
			if (compare_(value, current->data_))
			{
				current = current->left_.get();
			}
			else if (compare_(current->data_, value))
			{
				current = current->right_.get();
			}
			else
			{
				return Iterator(current);
			}
		}

		return end();
	}

	Iterator Min() const { return begin(); }

	Iterator Max() const { return Iterator(GetMaxNode(root_.get())); }

	Iterator Predecessor(Iterator it) const { return (it == end()) ? Max() : --it; }

	Iterator Predecessor(const T &value) const { return Predecessor(Find(value)); }

	Iterator Successor(Iterator it) const { return (it == end()) ? end() : ++it; }

	Iterator Successor(const T &value) const { return Successor(Find(value)); }

	template<typename Func>
	void ForEach(Func &&callback) const
	{
		Node::InOrderTraversal(root_.get(), std::forward<Func>(callback));
	}

	[[nodiscard]] size_t Size() const { return root_ ? root_->size_ : 0; }

	// insert value into the BST, if value already exists, return iterator to the existing node
	Iterator Insert(const T &value)
	{
		if (!root_)
		{
			root_ = std::make_unique<Node>(value);
			return Iterator(root_.get());
		}

		Node *parent = nullptr;
		Node *current = root_.get();
		bool isLeftChild = false;
		while (current)
		{
			parent = current;
			if (compare_(value, current->data_))
			{
				current = current->left_.get();
				isLeftChild = true;
			}
			else if (compare_(current->data_, value))
			{
				current = current->right_.get();
				isLeftChild = false;
			}
			else
			{
				// if exists, just return the iterator to the existing node
				return Iterator(current);
			}
		}

		std::unique_ptr<Node> newNode = std::make_unique<Node>(value, parent);
		Node *newNodePtr = newNode.get();
		if (isLeftChild)
		{
			parent->left_ = std::move(newNode);
		}
		else
		{
			parent->right_ = std::move(newNode);
		}

		UpdatePathSize(parent);
		return Iterator(newNodePtr);
	}

	void Delete(const T &value)
	{
		Iterator it = Find(value);
		if (it == end())
		{
			return; // value not found, nothing to delete
		}

		Delete(it.current_);
	}

	Iterator begin() const { return Iterator(GetMinNode(root_.get())); }

	Iterator end() const { return Iterator(nullptr); }

private:
	void Delete(Node *nodeToDelete)
	{
		if (!nodeToDelete->left_ && !nodeToDelete->right_)
		{
			// case 1: node has no children
			if (nodeToDelete == root_.get())
			{
				root_.reset();
			}
			else
			{
				Node *parent = nodeToDelete->parent_;
				if (parent->left_.get() == nodeToDelete)
				{
					parent->left_.reset();
				}
				else
				{
					parent->right_.reset();
				}

				UpdatePathSize(parent);
			}
		}
		else if (nodeToDelete->left_ && nodeToDelete->right_)
		{
			// case 3: node has two children
			Node *predecessor = GetPredecessorNode(nodeToDelete);
			std::swap(nodeToDelete->data_, predecessor->data_);
			Delete(predecessor);
		}
		else
		{
			// case 2: node has one child
			auto child = std::move(nodeToDelete->left_ ? nodeToDelete->left_ : nodeToDelete->right_);
			if (nodeToDelete == root_.get())
			{
				root_ = std::move(child);
				root_->parent_ = nullptr;
			}
			else
			{
				Node *parent = nodeToDelete->parent_;
				if (parent->left_.get() == nodeToDelete)
				{
					parent->left_ = std::move(child);
					parent->left_->parent_ = parent;
				}
				else
				{
					parent->right_ = std::move(child);
					parent->right_->parent_ = parent;
				}

				UpdatePathSize(parent);
			}
		}
	}

	static Node *GetMinNode(Node *node)
	{
		while (node && node->left_)
		{
			node = node->left_.get();
		}

		return node;
	}

	static Node *GetMaxNode(Node *node)
	{
		while (node && node->right_)
		{
			node = node->right_.get();
		}

		return node;
	}

	static Node *GetPredecessorNode(Node *node)
	{
		if (node->left_)
		{
			return GetMaxNode(node->left_.get());
		}

		Node *parent = node->parent_;
		while (parent && node == parent->left_.get())
		{
			node = parent;
			parent = parent->parent_;
		}

		return parent;
	}

	static Node *GetSuccessorNode(Node *node)
	{
		if (node->right_)
		{
			return GetMinNode(node->right_.get());
		}

		Node *parent = node->parent_;
		while (parent && node == parent->right_.get())
		{
			node = parent;
			parent = parent->parent_;
		}

		return parent;
	}

	static void UpdatePathSize(Node *x)
	{
		while (x)
		{
			UpdateSize(x);
			x = x->parent_;
		}
	}

	static void UpdateSize(Node *x)
	{
		if (x)
		{
			int leftSize = x->left_ ? x->left_->size_ : 0;
			int rightSize = x->right_ ? x->right_->size_ : 0;
			x->size_ = 1 + leftSize + rightSize;
		}
	}

	// 内部递归查找 i-th 最小元素
	Node *Select(Node *x, int i) const
	{
		if (!x)
		{
			return nullptr;
		}
		int leftSize = x->left_ ? x->left_->size_ : 0;
		int k = leftSize + 1; // 当前节点在当前子树中的排名

		if (i == k)
		{
			return x;
		}
		if (i < k)
		{
			return Select(x->left_.get(), i);
		}
		else
		{
			return Select(x->right_.get(), i - k);
		}
	}

private:
	std::unique_ptr<Node> root_;
	Compare compare_;
};

} // namespace guozi::tree
