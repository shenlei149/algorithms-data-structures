#include <stdexcept>
#include <vector>

namespace guozi::basic
{

template<typename T, typename Less = std::less<T>>
class Heap
{
public:
	explicit Heap(Less less = Less {})
		: less_(less)
	{}

	Heap(std::vector<T> data, Less less = Less {})
		: less_(less)
		, data_(std::move(data))
	{
		Heapify();
	}

	template<std::input_iterator Iterator>
	Heap(Iterator begin, Iterator end, Less less = Less {})
		: less_(less)
		, data_(begin, end)
	{
		Heapify();
	}

	T Top() const
	{
		if (data_.empty())
		{
			throw std::out_of_range("Heap is empty");
		}

		return data_.front();
	}

	void Pop()
	{
		if (data_.empty())
		{
			throw std::out_of_range("Heap is empty");
		}

		if (data_.size() > 1)
		{
			data_.front() = std::move(data_.back());
		}

		data_.pop_back();
		if (!data_.empty())
		{
			HeapifyDown(0);
		}
	}

	void Push(T value)
	{
		data_.push_back(std::move(value));
		HeapifyUp(data_.size() - 1);
	}

private:
	void Heapify()
	{
		if (data_.size() > 1)
		{
			for (int i = (data_.size() / 2) - 1; i >= 0; i--)
			{
				HeapifyDown(i);
			}
		}
	}

	void HeapifyUp(size_t index)
	{
		while (index > 0)
		{
			size_t parent = (index - 1) / 2;
			if (less_(data_[index], data_[parent]))
			{
				std::swap(data_[index], data_[parent]);
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
		size_t size = data_.size();
		while (true)
		{
			size_t left = 2 * index + 1;
			size_t right = 2 * index + 2;
			size_t smallest = index;

			if (left < size && less_(data_[left], data_[smallest]))
			{
				smallest = left;
			}
			if (right < size && less_(data_[right], data_[smallest]))
			{
				smallest = right;
			}

			if (smallest != index)
			{
				std::swap(data_[index], data_[smallest]);
				index = smallest;
			}
			else
			{
				break;
			}
		}
	}

private:
	Less less_;
	std::vector<T> data_;
};

} // namespace guozi::basic
