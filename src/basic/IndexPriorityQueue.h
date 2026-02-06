#include <stdexcept>
#include <vector>

namespace guozi::basic
{

template<typename T, typename Less = std::less<T>>
class IndexPriorityQueue
{
public:
	IndexPriorityQueue(size_t maxSize, Less less = Less {})
		: qp_(maxSize, static_cast<size_t>(-1))
		, data_(maxSize)
		, less_(less)
	{
		pq_.reserve(maxSize);
	}

	IndexPriorityQueue(std::vector<T> data, Less less = Less {})
		: qp_(data.size(), static_cast<size_t>(-1))
		, data_(std::move(data))
		, less_(less)
	{
		size_t n = data_.size();
		pq_.resize(n);
		for (size_t i = 0; i < n; ++i)
		{
			pq_[i] = i;
			qp_[i] = i;
		}

		if (n <= 1)
		{
			return;
		}

		// why use i-- > 0 instead of i >= 0; i-- ?
		// because size_t is unsigned, so i >= 0 is always true
		// and don't miss the i = 0 case
		for (size_t i = n / 2; i-- > 0;)
		{
			HeapifyDown(i);
		}
	}

	void Push(size_t index, T value)
	{
		if (index >= data_.size())
		{
			throw std::out_of_range("Index out of range");
		}

		if (qp_[index] != static_cast<size_t>(-1))
		{
			throw std::invalid_argument("Index already in priority queue");
		}

		data_[index] = std::move(value);
		size_t pos = pq_.size();
		pq_.push_back(index);
		qp_[index] = pos;
		HeapifyUp(pos);
	}

	void Upsert(size_t index, T value)
	{
		if (index >= data_.size())
		{
			throw std::out_of_range("Index out of range");
		}

		if (qp_[index] == static_cast<size_t>(-1))
		{
			Push(index, std::move(value));
		}
		else
		{
			data_[index] = std::move(value);
			HeapifyUp(qp_[index]);
			HeapifyDown(qp_[index]);
		}
	}

	void Delete(size_t index)
	{
		if (index >= data_.size())
		{
			throw std::out_of_range("Index out of range");
		}

		size_t pos = qp_[index];
		if (pos == static_cast<size_t>(-1))
		{
			throw std::invalid_argument("Index not in priority queue");
		}

		if (pos == pq_.size() - 1)
		{
			pq_.pop_back();
			qp_[index] = static_cast<size_t>(-1);
			return;
		}

		size_t lastIndex = pq_.back();
		pq_[pos] = lastIndex;
		qp_[lastIndex] = pos;
		pq_.pop_back();
		qp_[index] = static_cast<size_t>(-1);
		HeapifyUp(qp_[lastIndex]);
		HeapifyDown(qp_[lastIndex]);
	}

	size_t TopIndex() const { return pq_.front(); }

	void Pop() { Delete(pq_.front()); }

	const T &Top() const { return data_[pq_.front()]; }

	T &Top() { return data_[pq_.front()]; }

	bool Empty() const { return pq_.empty(); }

	size_t Size() const { return pq_.size(); }

	bool Contains(size_t index) const { return index < qp_.size() && qp_[index] != static_cast<size_t>(-1); }

private:
	void Exchange(size_t i, size_t j)
	{
		std::swap(pq_[i], pq_[j]);
		qp_[pq_[i]] = i;
		qp_[pq_[j]] = j;
	}

	void HeapifyUp(size_t index)
	{
		while (index > 0)
		{
			size_t parent = (index - 1) / 2;
			if (less_(data_[pq_[index]], data_[pq_[parent]]))
			{
				Exchange(index, parent);
				index = parent;
			}
			else
			{
				break;
			}
		}
	}

	void HeapifyDown(size_t index)
	{
		size_t size = pq_.size();
		while (true)
		{
			size_t left = 2 * index + 1;
			size_t right = 2 * index + 2;
			size_t smallest = index;

			if (left < size && less_(data_[pq_[left]], data_[pq_[smallest]]))
			{
				smallest = left;
			}
			if (right < size && less_(data_[pq_[right]], data_[pq_[smallest]]))
			{
				smallest = right;
			}

			if (smallest != index)
			{
				Exchange(index, smallest);
				index = smallest;
			}
			else
			{
				break;
			}
		}
	}

private:
	std::vector<size_t> pq_; // binary heap using 0-based indexing
	std::vector<size_t> qp_; // inverse: qp_[i] gives position of i in pq_
	std::vector<T> data_;
	Less less_;
};

} // namespace guozi::basic
