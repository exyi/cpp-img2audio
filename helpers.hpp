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

// http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
// of course, it could be implemented more effectively using Count Leading Zeros instruction, but C++ probably does not provide that in standard library... A perf oriented language they told :(
u32 next_power_of_two(u32 v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

#define COMPARE_LT_BY(shit) [&](let& left, let& right) { return ([&](let& it) { return shit; })(left) < ([&](let& it) { return shit; })(right); }
#define COMPARE_GT_BY(shit) [&](let& left, let& right) { return ([&](let& it) { return shit; })(left) > ([&](let& it) { return shit; })(right); }

// C++ iterators are just a great way to obscure the behavior (and errors) even more
#define ITERATE(iterable) (iterable).begin(), (iterable).end()
#define CREATE_VEC(Type, i, range_upperbound, expr) ([&]() { vector<Type> result_hackvector___; for (usize i = 0; i < range_upperbound; i++) result_hackvector___.emplace_back(expr); return result_hackvector___; })()

#define MUTABLE_LET_BECAUSE_CPP_COMMITTEE_ARE_IDIOTS_AND_WE_CANT_MOVE_CONST_VARS auto

// template <typename Function>
// struct FnOperation {
//     Function f;
//     FnOperation(Function f_) : f{f_} {}
// };

// template <typename X, typename Op>
// auto operator | (X&& data, FnOperation<Op> op) {
//     return forward(op.f(forward(data));
// }

// template 

template<typename F>
auto vec_init(usize count, F f) {
    using A = decltype(f((usize)1));
    return CREATE_VEC(A, i, count, move(f(i)));
}

template<typename TFrom, typename F>
auto vec_map(const vector<TFrom>& a, F f) {
    return CREATE_VEC(decltype(f(a[0])), i, a.size(), move(f(a.at(i))));
    //                                                ^ this can't be forward, because it does not work...
}

template<typename TFrom, typename F>
auto vec_mapi(const vector<TFrom>& a, F f) {
    return CREATE_VEC(decltype(f(1, a[0])), i, a.size(), move(f(i, a.at(i))));
    //                                                ^ this can't be forward, because it does not work...
}

template<typename TFrom, typename F>
auto vec_map_into(vector<TFrom>&& a, F f) {
    return CREATE_VEC(decltype(f(a[0])), i, a.size(), forward(f(move(a.at(i)))));
}

template<typename T>
vector<T> vec_limit(vector<T>&& a, usize size) {
    return CREATE_VEC(T, i, min(size, a.size()), move(a.at(i)));
}

f64 random_number() {
    // f64 lower_bound = 0;
    // f64 upper_bound = 1;
    // uniform_real_distribution<f64> unif(lower_bound,upper_bound);
    // default_random_engine re;
    // return unif(re);

    // ^ toto je implementace pomocí skvělé nové moderní C++ knihovny. Takže je to dlouhé, nerozumím tomu, a navíc to vrací pořád to stejné číslo...

    return (f64)rand() / RAND_MAX;
}

// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
