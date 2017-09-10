#pragma once
#include <iostream>
#include <cmath>
#include <random>
#include <type_traits>

namespace tiny_cnn {

typedef float double_t;

typedef std::size_t cnn_size_t;

typedef std::vector<double_t> vec_t;

template <typename T> inline
typename std::enable_if<std::is_integral<T>::value, T>::type
uniform_rand(T min, T max) {
    // 随机种子设置为静态变量, std::mt19937 为一个 engine
    static std::mt19937 gen(1);
    std::uniform_int_distribution<T> dst(min, max);
    return dst(gen);
}

template <typename T> inline
typename std::enable_if<std::is_floating_point<T>::value, T>::type
uniform_rand(T min, T max) {
    static std::mt19937 gen(1);
    std::uniform_real_distribution<T> dst(min, max);
    return dst(gen);
}

template <typename T> inline
typename std::enable_if<std::is_floating_point<T>::value, T>::type
gaussian_rand(T mean, T sigma) {
    static std::mt19937 gen(1);
    std::normal_distribution<T> dst(mean, sigma);
    return dst(gen);
}

template <typename Iter>
void uniform_rand(Iter begin, Iter end, double_t min, double_t max) {
    for (Iter it = begin; it != end; ++it) 
        *it = uniform_rand(min, max);
}

} // namespace tiny_cnn
