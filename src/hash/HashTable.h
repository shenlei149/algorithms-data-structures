#pragma once

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <functional>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace guozi::hash
{

template<typename K, typename V>
class ChainPolicy
{
public:
	struct Entry
	{
		K key_;
		[[no_unique_address]] V value_;
	};

	using BucketType = std::list<Entry>;

	static constexpr bool SupportsDeletion = true;
	static constexpr float DefaultLoadFactor = 0.75f;
	static constexpr float ShrinkMultiplier = 0.33f;

	template<typename KT, typename VT>
	static bool Insert(std::vector<BucketType> &table, KT &&key, VT &&value, size_t index)
	{
		for (auto &entry : table[index])
		{
			if (entry.key_ == key)
			{
				entry.value_ = std::forward<VT>(value);
				return false;
			}
		}

		table[index].push_back({ std::forward<KT>(key), std::forward<VT>(value) });
		return true;
	}

	template<typename Table, typename ValuePtr>
	static bool Find(Table &table, const K &key, size_t index, ValuePtr **valuePtr)
	{
		auto &bucket = table[index];
		for (auto &entry : bucket)
		{
			if (entry.key_ == key)
			{
				if (valuePtr)
				{
					*valuePtr = &entry.value_;
				}

				return true;
			}
		}

		return false;
	}

	static bool Remove(std::vector<BucketType> &table, const K &key, size_t index)
	{
		auto &bucket = table[index];
		for (auto it = bucket.begin(); it != bucket.end(); ++it)
		{
			if (it->key_ == key)
			{
				bucket.erase(it);
				return true;
			}
		}

		return false;
	}
};

template<typename K>
class DefaultHashFunc2
{
public:
	size_t operator()(const K &key) const { return std::hash<K> {}(key); }
};

template<typename K, typename V, typename HashFunc2 = DefaultHashFunc2<K>>
class DoubleHashPolicy
{
public:
	enum class EntryState
	{
		Empty,
		Occupied,
		Deleted
	};

	struct Entry
	{
		K key_;
		[[no_unique_address]] V value_;
		EntryState state_ = EntryState::Empty;
	};

	using BucketType = Entry;

	static constexpr bool SupportsDeletion = true;
	static constexpr float DefaultLoadFactor = 0.5f;
	static constexpr float ShrinkMultiplier = 0.25f;

	template<typename KT, typename VT>
	static bool Insert(std::vector<BucketType> &table, KT &&key, VT &&value, size_t index)
	{
		size_t capacity = table.size();
		size_t step = GetProbeStep(key);
		size_t firstDeletedIndex = capacity;
		for (size_t i = 0; i < capacity; ++i)
		{
			auto &slot = table[index];
			if (slot.state_ == EntryState::Occupied && slot.key_ == key)
			{
				slot.value_ = std::forward<VT>(value);
				return false;
			}

			if (slot.state_ == EntryState::Deleted && firstDeletedIndex == capacity)
			{
				firstDeletedIndex = index;
			}

			if (slot.state_ == EntryState::Empty)
			{
				size_t targetIndex = firstDeletedIndex == capacity ? index : firstDeletedIndex;
				auto &target = table[targetIndex];
				target.key_ = std::forward<KT>(key);
				target.value_ = std::forward<VT>(value);
				target.state_ = EntryState::Occupied;
				return true;
			}

			index = (index + step) % capacity;
		}

		if (firstDeletedIndex != capacity)
		{
			auto &target = table[firstDeletedIndex];
			target.key_ = std::forward<KT>(key);
			target.value_ = std::forward<VT>(value);
			target.state_ = EntryState::Occupied;
			return true;
		}

		return false;
	}

	template<typename Table, typename ValuePtr>
	static bool Find(Table &table, const K &key, size_t index, ValuePtr **valuePtr)
	{
		size_t capacity = table.size();
		size_t step = GetProbeStep(key);
		for (size_t i = 0; i < capacity; ++i)
		{
			auto &slot = table[index];
			if (slot.state_ == EntryState::Empty)
			{
				return false;
			}

			if (slot.state_ == EntryState::Occupied && slot.key_ == key)
			{
				if (valuePtr)
				{
					*valuePtr = &slot.value_;
				}

				return true;
			}

			index = (index + step) % capacity;
		}

		return false;
	}

	static bool Remove(std::vector<BucketType> &table, const K &key, size_t index)
	{
		size_t capacity = table.size();
		size_t step = GetProbeStep(key);
		for (size_t i = 0; i < capacity; ++i)
		{
			auto &slot = table[index];
			if (slot.state_ == EntryState::Empty)
			{
				return false;
			}

			if (slot.state_ == EntryState::Occupied && slot.key_ == key)
			{
				slot.state_ = EntryState::Deleted;
				return true;
			}

			index = (index + step) % capacity;
		}

		return false;
	}

private:
	static size_t GetProbeStep(const K &key)
	{
		size_t step = HashFunc2 {}(key);
		return ((step % 7 + 1) << 1) | 1; // Ensure step is odd and go through all buckets
	}
};

template<typename K, typename V, typename Policy, typename HashFunc = std::hash<K>>
class HashTable
{
public:
	using BucketType = typename Policy::BucketType;

	HashTable(size_t hint, float loadFactor = Policy::DefaultLoadFactor, size_t minCapacity = 8)
		: hash_()
		, minCapacity_(std::bit_ceil(std::max<size_t>(1, minCapacity)))
		, loadFactor_(loadFactor)
	{
		if (!(loadFactor_ > 0.0f && loadFactor_ < 1.0f))
		{
			throw std::invalid_argument("loadFactor must be in (0, 1)");
		}

		size_t capacity = std::bit_ceil(std::max(minCapacity_, static_cast<size_t>(hint / loadFactor_)));
		buckets_.resize(capacity);
		UpdateThreshold(capacity);
	}

	template<typename KT, typename VT>
	std::enable_if_t<!std::is_same_v<V, std::monostate> && std::is_constructible_v<K, KT &&> &&
					 std::is_constructible_v<V, VT &&>>
	Insert(KT &&key, VT &&value)
	{
		CheckSize();
		InsertImpl(std::forward<KT>(key), std::forward<VT>(value));
	}

	template<typename KT, typename VT = V>
	std::enable_if_t<std::is_same_v<VT, std::monostate> && std::is_constructible_v<K, KT &&>> Insert(KT &&key)
	{
		CheckSize();
		InsertImpl(std::forward<KT>(key));
	}

	bool Contains(const K &key) const
	{
		auto index = hash_(key) % buckets_.size();
		return Policy::Find(buckets_, key, index, static_cast<const V **>(nullptr));
	}

	template<typename Self, typename VT = V>
	std::enable_if_t<!std::is_same_v<VT, std::monostate>,
					 std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const V *, V *>>
	Get(this Self &&self, const K &key)
	{
		auto &buckets = self.buckets_;
		auto index = self.hash_(key) % buckets.size();

		using OutPtrType = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const V *, V *>;
		OutPtrType outPtr = nullptr;
		if (Policy::Find(buckets, key, index, &outPtr))
		{
			return outPtr;
		}

		return nullptr;
	}

	bool Remove(const K &key)
	{
		if constexpr (!Policy::SupportsDeletion)
		{
			return false;
		}
		else
		{
			auto index = hash_(key) % buckets_.size();
			if (Policy::Remove(buckets_, key, index))
			{
				--size_;
				CheckSize();
				return true;
			}

			return false;
		}
	}

	size_t Size() const { return size_; }

	size_t Capacity() const { return buckets_.size(); }

private:
	template<typename KT, typename VT>
	void InsertImpl(KT &&key, VT &&value)
	{
		auto index = hash_(key) % buckets_.size();
		if (Policy::Insert(buckets_, std::forward<KT>(key), std::forward<VT>(value), index))
		{
			++size_;
		}
	}

	template<typename KT>
	void InsertImpl(KT &&key)
	{
		auto index = hash_(key) % buckets_.size();
		if (Policy::Insert(buckets_, std::forward<KT>(key), V {}, index))
		{
			++size_;
		}
	}

	void UpdateThreshold(size_t capacity)
	{
		maxThreshold_ = std::max<size_t>(1, static_cast<size_t>(capacity * loadFactor_));
		minThreshold_ = static_cast<size_t>(capacity * Policy::ShrinkMultiplier);
	}

	void CheckSize()
	{
		if (size_ >= maxThreshold_)
		{
			Resize(buckets_.size() * 2);
		}

		if (size_ < minThreshold_ && buckets_.size() > minCapacity_)
		{
			Resize(buckets_.size() / 2);
		}
	}

	void Resize(size_t newCapacity)
	{
		if (newCapacity < minCapacity_)
		{
			newCapacity = minCapacity_;
		}

		std::vector<BucketType> oldBuckets = std::move(buckets_);
		buckets_.assign(newCapacity, BucketType {});
		size_ = 0;
		UpdateThreshold(newCapacity);

		for (auto &bucket : oldBuckets)
		{
			if constexpr (std::is_same_v<BucketType, typename ChainPolicy<K, V>::BucketType>)
			{
				for (auto &entry : bucket)
				{
					Insert(std::move(entry));
				}
			}
			else
			{
				if (bucket.state_ == DoubleHashPolicy<K, V>::EntryState::Occupied)
				{
					Insert(std::move(bucket));
				}
			}
		}
	}

	void Insert(typename Policy::Entry &&entry)
	{
		if constexpr (std::is_same_v<V, std::monostate>)
		{
			InsertImpl(std::move(entry.key_));
		}
		else
		{
			InsertImpl(std::move(entry.key_), std::move(entry.value_));
		}
	}

private:
	std::vector<BucketType> buckets_;
	size_t size_ = 0;
	HashFunc hash_;

	size_t maxThreshold_;
	size_t minThreshold_;
	size_t minCapacity_;
	float loadFactor_;
};

template<typename K, typename V>
using ChainHashMap = HashTable<K, V, ChainPolicy<K, V>>;

template<typename K, typename V>
using DoubleHashMap = HashTable<K, V, DoubleHashPolicy<K, V>>;

template<typename K>
using ChainHashSet = ChainHashMap<K, std::monostate>;

template<typename K>
using DoubleHashSet = DoubleHashMap<K, std::monostate>;

} // namespace guozi::hash
