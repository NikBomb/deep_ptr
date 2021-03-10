#pragma once

// CRTP Interfaces for cloneable pattern. 
// Set-up: The base class should derive from Cloneable
// The derived class (One level of hierarchy supported) should derive directly 
// from Cloner<Base, Derived> and not from Base.
// Example:

// class A : public Cloneable<A>{  //Your implementation here }
// class B : public Cloner<A , B>{ //Your implementation here }

template <typename Base>
class Cloneable {
	virtual deep_ptr<Base> clone_impl() const = 0;
public:
	virtual deep_ptr<Base> clone() const { return clone_impl(); };
	virtual ~Cloneable() = default;
};


template<typename Base, typename Derived>
class Cloner : public Base {
	
	Derived* clone_helper() const {
		return new Derived(static_cast<const Derived&>(*this));
	}

    virtual deep_ptr<Base> clone_impl() const override
	{
		std::cout << "Calling Clone from interface" << '\n';
		return deep_ptr<Base>(clone_helper());
	}
public:
	deep_ptr<Derived> pure_clone() const {
		std::cout << "Calling Clone from derived" << '\n';
		return deep_ptr<Derived>(clone_helper());
	};
};
