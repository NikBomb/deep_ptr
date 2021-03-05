#pragma once
// deep_ptr implementation 

// Wrapper around unique_ptr: deep_ptr allows the safety of a unique ptr with copy semantics.
// When a copy is required the deep_ptr copies the resource. 
// To avoid object slicing in a class hierarchy ann object of type T
// has to provide an accessible T* clone() method, which will be chosen over the copy constructor.  

#include <memory>

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
	T* copy_or_clone(T* ptr) {
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

	deep_ptr(const deep_ptr& other) : std::unique_ptr<T>(details::copy_or_clone(other.get())) {};

	template <typename U>
	explicit deep_ptr(const std::unique_ptr<U>& other)
		: std::unique_ptr<T>(details::copy_or_clone(other.get())) {}

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
		std::unique_ptr<T>::reset(details::copy_or_clone(&other));
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
			std::unique_ptr<T>::reset(copy_or_clone(src.get()));
		}
		return *this;
	}

	template <typename U>
	deep_ptr& operator=(const std::unique_ptr<U>& other) {
		std::unique_ptr<T>::reset(details::copy_or_clone(other.get()));
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

