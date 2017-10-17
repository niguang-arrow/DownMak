#include <iostream>
#include <cassert>

template <typename T>
struct index3d {

    index3d() :
        width_(0), height_(0), depth_(0) {}

    index3d(T width, T height, T depth) :
        width_(width), height_(height), depth_(depth) {}

    T get_index(T x, T y, T channel) const {
        assert(x >= 0 && x < width_);
        assert(y >= 0 && y < height_);
        assert(depth >= 0 && depth < depth_);

        return (y + channel * height_) * width_ + x;
    }

    T area() const {
        return height_ * width_;
    }

    T size() const {
        return height_ * width_ * depth_;
    }

    T width_;
    T height_;
    T depth_;
};
