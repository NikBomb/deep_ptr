#pragma once

#define CLONEABLE(Type) \
    virtual Type *clone() const = 0;

#define CLONER(Type) \
    virtual Type *clone() const { std::cout << "Calling Clone" << '\n'; \
    return new Type(*this); }


template <typename Base>
class Cloneable {
	virtual Base* clone_impl() const = 0;
public:
	virtual Base* clone() { return clone_impl(); };
};


template<typename Base, typename Derived>
class Cloner : public Base {
	virtual Base* clone_impl() const
	{
		std::cout << "Calling Clone from interface" << '\n';
		return new Derived(static_cast<const Derived&>(*this));
	}
public:
	virtual Derived* clone() const {
		std::cout << "Calling Clone from derived" << '\n';
		return static_cast<Derived*>(clone_impl());
	};
};
