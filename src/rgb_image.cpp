#include "rgb_image.hpp"

namespace isp {

RgbImage::RgbImage(int width, int height, int bit_depth)
    : width_(width)
    , height_(height)
    , bit_depth_(bit_depth)
    , data_(static_cast<std::size_t>(width * height))
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
    }
}

uint16_t RgbImage::max_value() const {
    return static_cast<uint16_t>((1 << bit_depth_) - 1);
}

Pixel& RgbImage::at(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return data_[static_cast<std::size_t>(y * width_ + x)];
}

const Pixel& RgbImage::at(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return data_[static_cast<std::size_t>(y * width_ + x)];
}

} // namespace isp