#pragma once

#include <memory>
#include "type_traits"
namespace details {


	template <typename T>
	class has_clone
	{
	private:
		typedef char Yes[1];
		typedef char No[2];


		template <typename C> static constexpr Yes& test(decltype(&C::clone)) {};
		template <typename C> static constexpr No& test(...) {};
	public:
		static constexpr bool value = std::is_same_v<Yes&, decltype(test<T>(0))>;
	};

	template <typename T>
	T* copy(T* ptr) {
		if (ptr) {
			if constexpr (has_clone<T>::value) {
				return ptr->clone();
			}
			else {
				return  new T(*ptr);
			}
		}
		else {
			return nullptr;
		}
	}
}

template <typename T, typename Del = std::default_delete<T>>
class deep_ptr : public std::unique_ptr<T> {

public:
	deep_ptr() noexcept : std::unique_ptr<T>() {};

	explicit deep_ptr(T* obj) noexcept : std::unique_ptr<T>(obj) {};

	deep_ptr(const deep_ptr& other) : std::unique_ptr<T>(details::copy(other.get())) {};

	template <typename U>
	explicit deep_ptr(const std::unique_ptr<U>& other)
		: std::unique_ptr<T>(details::copy(other.get())) {}

	deep_ptr(deep_ptr&& other) noexcept
		: std::unique_ptr<T>(other.release()) {}

	explicit deep_ptr(std::unique_ptr<T>&& other) noexcept
		: std::unique_ptr<T>(other.release()) {}

	template <typename U>
	explicit deep_ptr(std::unique_ptr<U>&& other) noexcept
		: std::unique_ptr<T>(other.release()) {}

	deep_ptr& operator=(T* other) noexcept {
		std::unique_ptr<T>::reset(other);
		return *this;
	}

	deep_ptr& operator=(const T& other) {
		std::unique_ptr<T>::reset(details::copy(&other));
		return *this;
	}

	deep_ptr& operator=(const deep_ptr& other) {
		return operator=(static_cast<const std::unique_ptr<T>&>(other));
	}

	template <typename U>
	deep_ptr& operator=(const deep_ptr<U>& other) {
		return operator=(static_cast<const std::unique_ptr<U>&>(other));
	}

	deep_ptr& operator=(const std::unique_ptr<T>& src) {
		if (&src != this) {
			std::unique_ptr<T>::reset(copy(src.get()));
		}
		return *this;
	}

	template <typename U>
	deep_ptr& operator=(const std::unique_ptr<U>& other) {
		std::unique_ptr<T>::reset(details::copy(other.get()));
		return *this;
	}

	deep_ptr& operator=(deep_ptr&& other) noexcept {
		std::unique_ptr<T>::reset(other.release());
		return *this;
	}

	template <typename U>
	deep_ptr& operator=(deep_ptr<U>&& other) noexcept {
		std::unique_ptr<T>::reset(other.release());
		return *this;
	}


	deep_ptr& operator=(std::unique_ptr<T>&& other) noexcept {
		std::unique_ptr<T>::reset(other.release());
		return *this;
	}


	template <typename U>
	deep_ptr& operator=(std::unique_ptr<U>&& other) noexcept {
		std::unique_ptr<T>::reset(other.release());
		return *this;
	};
	
	bool empty() const noexcept { return !(*this); }

	T* get() const noexcept { return std::unique_ptr<T>::get(); }
};

// C++14 make_unique
namespace detail {
	template<class>
	constexpr bool is_unbounded_array_v = false;
	template<class T>
	constexpr bool is_unbounded_array_v<T[]> = true;

	template<class>
	constexpr bool is_bounded_array_v = false;
	template<class T, std::size_t N>
	constexpr bool is_bounded_array_v<T[N]> = true;
} // namespace detail


template<class T, class... Args>
std::enable_if_t<!std::is_array<T>::value, deep_ptr<T>>
make_deep(Args&&... args)
{
	return deep_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
std::enable_if_t<detail::is_unbounded_array_v<T>, deep_ptr<T>>
make_deep(std::size_t n)
{
	return deep_ptr<T>(new std::remove_extent_t<T>[n]());
}

template<class T, class... Args>
std::enable_if_t<detail::is_bounded_array_v<T>> make_deep(Args&&...) = delete;