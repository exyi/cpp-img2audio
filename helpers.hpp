#pragma once
#include "my_std.hpp"

template<typename T>
vector<T> make_vector() { return vector<T>(); }

// same as vector{...} initialized except that this thing works with types without the f***ing implicit copy constructor
template<typename T, typename ...Ts>
vector<T> make_vector(T&& arg1, Ts&& ...args) {
    auto v = make_vector<T>(std::forward<T>(args)...);
    v.emplace_back(std::forward<T>(arg1));
    return v;
}

#define COMPARE_LT_BY(shit) [&](let& left, let& right) { return ([&](let& it) { return shit; })(left) < ([&](let& it) { return shit; })(right); }
#define COMPARE_GT_BY(shit) [&](let& left, let& right) { return ([&](let& it) { return shit; })(left) > ([&](let& it) { return shit; })(right); }

#define ITERATE(iterable) (iterable).begin(), (iterable).end()
