#pragma once
#include "my_std.hpp"
#include "helpers.hpp"
#include <complex>

auto get_first_omega(i32 n) {
    let angle = (2 * M_PI) / n;
    using namespace complex_literals;
    return exp(1i * angle);
}

template<typename T>
auto vector_stride(const vector<T>& vec, usize from_index, usize stride_size) {
    vector<T> result;
    for (usize i = from_index; i < vec.size(); i += stride_size) {
        result.emplace_back(vec.at(i));
    }

    return result;
}

auto fft_complex(vector<complex<f64>> p, complex<f64> omega) {
    // assert(isclose(abs(omega), 1))
    if (p.size() == 1)
        return p;
    if (p.size() % 2 == 1)
        throw runtime_error("FFT expects the p.size() to be a power of 2.");

    let ss = fft_complex(vector_stride(p, 0, 2), pow(omega, 2));
    let ls = fft_complex(vector_stride(p, 1, 2), pow(omega, 2));

    assert(ss.size() == ls.size());

    vector<complex<f64>> result;

    for (usize j = 0; j < ss.size(); j++) {
        let l = ls[j];
        let s = ss[j];
        result.emplace_back(
            s + pow(omega, j) * l
        );
    }

    for (usize j = 0; j < ss.size(); j++) {
        let l = ls[j];
        let s = ss[j];
        result.emplace_back(
            s - pow(omega, j) * l
        );
    }

    return result;
}

auto fft_complex(vector<complex<f64>> p) {
    return fft_complex(p, get_first_omega(p.size()));
}

auto fft_real(vector<f64> p) {
    vector<complex<f64>> p_c;
    using namespace complex_literals;
    for (usize i = 0; i < p.size(); i++)
        p_c.emplace_back(1i * p.at(i));
    for (usize i = p.size() - 1; i < (usize)-1; i--)
        p_c.emplace_back(-1i * p.at(i));

    let result_c = fft_complex(p_c);
    vector<f64> result;
    for (usize i = 0; i < result_c.size(); i++)
        result.emplace_back(abs(result_c.at(i)));
    return result;
}

auto ifft_complex(vector<complex<f64>> p) {
    let r1 = fft_complex(p, pow(get_first_omega(p.size()), -1));
    vector<complex<f64>> r2;
    for (usize i = 0; i < r1.size(); i++)
        r2.emplace_back(r1.at(i) / (f64)p.size());
    return r2;
}

auto ifft_real(vector<f64> p) {
    vector<complex<f64>> p_c;
    using namespace complex_literals;
    for (usize i = 0; i < p.size(); i++)
        p_c.emplace_back(p.at(i));
    p_c.emplace_back(0.0 + 0i);
    for (usize i = p.size() - 1; i != 0; i--)
        p_c.emplace_back(-1 * p.at(i));

    // cerr << "Invoking fft " << p_c.size() << "\n";
    let result_c = ifft_complex(p_c);
    vector<f64> result;
    for (usize i = 0; i < result_c.size(); i++)
        result.emplace_back(result_c.at(i).imag());
    return result;
}

auto ifft_real_random_phase(vector<f64> p) {
    vector<complex<f64>> p_c;
    using namespace complex_literals;
    for (usize i = 0; i < p.size(); i++)
        p_c.emplace_back(polar(p.at(i), random_number() * M_PI * 2));
    p_c.emplace_back(0.0 + 0i);
    for (usize i = p.size() - 1; i != 0; i--)
        p_c.emplace_back(-1.0 * p_c.at(i));

    // cerr << "Invoking fft " << p_c.size() << "\n";
    let result_c = ifft_complex(p_c);
    vector<f64> result;
    for (usize i = 0; i < result_c.size(); i++)
        result.emplace_back(result_c.at(i).imag());
    return result;
}

// def mul_pol(p, q):
//     gsize = 2 ** math.ceil(math.log2(len(p) + len(q)))
//     p += [0] * (gsize - len(p))
//     q += [0] * (gsize - len(p))
//     graph = [ a * b for (a, b) in zip(fft(p), fft(q)) ]
//     return ifft(graph)

// print(mul_pol([3, 1, 4], [1, 5, 9]))
