#pragma once

#include <memory>

namespace details {
	
	typedef char Yes[1];
	typedef char No[2];

	template <typename T>
	class has_clone
	{
	private:

		
		template <typename C> static constexpr Yes& test(decltype(&C::clone));
		template <typename C> static constexpr No& test(...);
	public:
		enum { value = sizeof(test<T>(0)) == sizeof(Yes) };
	};


	template <typename T>
	auto copy_impl(const T* ptr, std::true_type) {
		return ptr ? nullptr : ptr->clone().release();
	}

	template <typename T>
	auto copy_impl(const T* ptr, std::false_type) {
		return ptr ? nullptr : new T(*ptr);
	}

	template <typename T>
	auto copy(const T* ptr) {
		return copy_impl(ptr, has_clone<T>::value);
	}
}

template <typename T>
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

};
