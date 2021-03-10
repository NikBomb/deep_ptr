#include <iostream>
#include "deep_ptr.h"
#include "Cloneable.h"



struct Icomputer : public Cloneable<Icomputer> {
	
	virtual double compute() = 0;
	virtual ~Icomputer() = default;
};

class Computer : public Cloner<Icomputer, Computer> {
public:

	struct CompInput {
		double n1;
		double n2;
	};
	
	Computer() {};
	Computer(double n1, double n2) : inp{n1, n2 } {
		std::cout << "Calling Constructor" << "\n";
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
	
	// Create a IComputer using deep ptr and copy it as interface 
	deep_ptr<Icomputer> c = make_deep<Computer>(1, 2);
	auto user = compUser(c);
	auto copied_user = user;  //This is legal syntax because the member interface is a deep_ptr
	auto res = copied_user.compute();
	// Create a deep_ptr of concrete class clone it as deep ptr and use it
	deep_ptr<Computer> k = make_deep<Computer>(1, 2);
	auto l = k->pure_clone();
	auto res2 = l->compute();

	// Move the original user 
	auto moved_user = std::move(user);  // User will be in a legal state but the deep_ptr to the interface is null

	// The number of calls to clone and constructor have to be equal to the number of calls to destructor
}
