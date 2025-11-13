#pragma once

#include <type_traits>
#include <vector>
#include <memory>
#include <array>
#include <cassert>

/**
* std::is_integral_v<T> 编译期判断T是否为整型，否则返回false
* std::enable_if<条件，类型> 若条件为真则返回类型（省略为void），为假则替换失败，而不返回任何类型
* typename = 是一个匿名参数，仅用于类型检查
*/
template<typename T, size_t PageSize, typename = std::enable_if<std::is_integral_v<T>>>
class SparseSets final
{
public:
	void Add(T t)
	{
		density_.push_back(t);
		assure(t);
		index(t) = density_.size() - 1;
	}

	void Remove(T t)
	{
		if (!Contain(t)) return;

		auto& idx = index(t);
		if (idx == density_.size() - 1)
		{
			idx = null;
			density_.pop_back();
		}
		else
		{
			auto last = density_.back();
			index(last) = idx;
			std::swap(density_[idx], density_.back());
			idx = null;
			density_.pop_back();
		}
	}

	bool Contain(T t) const
	{
		assert(t != null);

		auto p = page(t);
		auto o = offset(t);

		return p < sparse_.size() && sparse_[p]->at(o) != null;
	}

	void Clear()
	{
		density_.clear();
		sparse_.clear();
	}

	auto begin() { return density_.begin(); }
	auto end() { return density_.end(); }

private:
	std::vector<T> density_;
	/**
	* 这里的 unique_ptr 保证了在使用时才会给对应的 array 分配内存
	*/
	std::vector<std::unique_ptr<std::array<T, PageSize>>> sparse_;
	static constexpr T null = std::numeric_limits<T>::max();

	size_t page(T t) const
	{
		return t / PageSize;
	}

	T index(T t) const
	{
		return sparse_[page(t)]->at(offset(t));
	}

	T& index(T t)
	{
		return sparse_[page(t)]->at(offset(t));
	}

	size_t offset(T t) const
	{
		return t % PageSize;
	}

	void assure(T t)
	{
		auto p = page(t);
		if (p >= sparse_.size())
		{
			for (size_t i = sparse_.size(); i <= p; i++)
			{
				sparse_.emplace_back(std::make_unique<std::array<T, PageSize>>());
				sparse_[i]->fill(null);
			}
		}
	}
};