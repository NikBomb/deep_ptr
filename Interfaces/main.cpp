#include <any>
#include <type_traits>

#include "memory"
#include <type_traits>

#include <iostream>
#include <type_traits>
#include "deep_ptr.h"


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
	Computer(double n1, double n2) : inp{n1, n2 } {
		std::cout << "Calling Constructor" << "\n";
	}

	virtual std::any getInput() {
		return std::any{&inp};
	};
	
	virtual Icomputer* clone() override {
		std::cout << "Calling clone"  << "\n";
		return new Computer(*this);
	}

	virtual double compute() {
		return inp.n1 + inp.n2;
	};

	virtual ~Computer() { std::cout << "calling destructor" << "\n"; }
private:
	CompInput inp;
};


class compUser {
	deep_ptr<Icomputer> m_comp;
public:
	compUser(const deep_ptr<Icomputer>& comp) : m_comp{ comp } {};
	double compute() {
		return m_comp->compute();
	}
};

int main() {
	
	//std::unique_ptr<Icomputer> a = std::make_unique<Computer>(1, 2);
	 //deep_ptr<Icomputer> c = make_deep<Computer>(1, 2);
	deep_ptr<Icomputer> c{ new Computer(1, 2) };
	auto user = compUser(c);
	auto copied_user = user;
	//
	auto res = copied_user.compute();
}
