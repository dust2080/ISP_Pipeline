#include "image.hpp"
#include <algorithm>

namespace isp {

Image::Image(int width, int height, int bit_depth, BayerPattern pattern)
    : width_(width)
    , height_(height)
    , bit_depth_(bit_depth)
    , pattern_(pattern)
    , data_(static_cast<std::size_t>(width * height))
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
    }
    if (bit_depth < 8 || bit_depth > 16) {
        throw std::invalid_argument("Bit depth must be between 8 and 16");
    }
}

uint16_t Image::max_value() const {
    return static_cast<uint16_t>((1 << bit_depth_) - 1);
}

uint16_t& Image::at(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return data_[static_cast<std::size_t>(y * width_ + x)];
}

const uint16_t& Image::at(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return data_[static_cast<std::size_t>(y * width_ + x)];
}

void Image::fill(uint16_t value) {
    std::fill(data_.begin(), data_.end(), value);
}

} // namespace isp