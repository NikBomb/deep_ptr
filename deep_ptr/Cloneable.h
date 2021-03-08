#pragma once

#define BASE_CLONEABLE(Type) \
    virtual Type *clone() const = 0;

#define CLONEABLE(Type) \
    virtual Type *clone() const { std::cout << "Calling Clone" << '\n'; \
    return new Type(*this); }