#include <any>
#include <type_traits>

#include "memory"
#include <type_traits>

#include <iostream>
#include <type_traits>
#include "deep_ptr.h"

//template <typename T>
//class has_clone
//{
//private:
//	typedef char YesType[1];
//	typedef char NoType[2];
//
//	template <typename C> static YesType& test(decltype(&C::clone));
//	template <typename C> static NoType& test(...);
//
//
//public:
//	enum { value = sizeof(test<T>(0)) == sizeof(YesType)};
//};
//
//template <typename T>
//struct deep_copy_ptr : public std::unique_ptr<T>
//{
//	using std::unique_ptr<T>::unique_ptr;
//	deep_copy_ptr() {};
//	deep_copy_ptr(deep_copy_ptr<T> const& other)
//	{
//		if constexpr(has_clone<T>::value) {
//			this->reset(other->clone());
//		}
//		else {
//			auto value = *other.get();
//			this->reset(new T(value));
//		}
//	}
//	deep_copy_ptr<T>& operator=(deep_copy_ptr<T> const& other)
//	{
//		if constexpr (has_clone<T>::value) {
//			this->reset(other->clone());
//		}
//		else {
//			auto value = *other.get();
//			this->reset(new T(value));
//		}
//		return *this;
//	}
//};

struct Icomputer {
	virtual std::any getInput() = 0;
	virtual double compute() = 0;
	virtual Icomputer* clone() = 0;
	virtual ~Icomputer() = default;
	
};


class Computer : public Icomputer{
public:
	struct CompInput {
		double n1;
		double n2;
	};
	
	Computer() {};
	Computer(double n1, double n2) : inp{n1, n2 } {}

	virtual std::any getInput() {
		return std::any{&inp};
	};
	
	virtual Icomputer* clone() override {
		return new Computer(*this);
	}

	virtual double compute() {
		return inp.n1 + inp.n2;
	};

private:
	CompInput inp;
};


class compUser {

	deep_ptr<Icomputer> m_comp;
public:
	compUser(deep_ptr<Icomputer> comp) : m_comp{ comp } {};
	double compute() {
		return m_comp->compute();
	}
};


void trial(std::true_type cond) {
	std::cout << "I am true";
}


void trial(std::false_type cond) {
	std::cout << "I am false";
}

int main() {

	trial(details::has_clone<int>::value);

	//static_assert(details::has_clone<int>::value == std::false_type{}, "Has clone");
	
	deep_ptr<Icomputer> c{ new Computer(1, 2) };

	//auto user = compUser(c);
	//auto copied_user = user;
	
	//auto res = copied_user.compute();
}
