#pragma once
#include "util.h"

namespace tiny_cnn {
namespace weight_init {

// function 为抽象基类
class function {
public:
    // fill 纯虚函数
    virtual void fill(vec_t *weight, cnn_size_t fan_in, cnn_size_t fan_out) = 0;
};

// scalable 也是一个抽象基类
class scalable : public function {
public:
    scalable(double_t value) : scale_(value) {}
    void scale(double_t value) {
        scale_ = value;
    }
protected:
    double_t scale_;
};

class xavier : public scalable {
public:
    xavier() : scalable(double_t(6)) {}
    explicit xavier(double_t value) : scalable(value) {}
    // sqrt 在 cmath
    void fill(vec_t *weight, cnn_size_t fan_in, cnn_size_t fan_out) override {
        const double_t weight_base = std::sqrt(scale_ / (fan_in + fan_out));
        uniform_rand(weight->begin(), weight->end(), -weight_base, weigth_base);
    }
};

} // namespace weight_init
} // namespace tiny_cnn
