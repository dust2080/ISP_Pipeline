#ifndef ISP_PIPELINE_RGB_IMAGE_HPP
#define ISP_PIPELINE_RGB_IMAGE_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>

namespace isp {

struct Pixel {
    uint16_t r{0};
    uint16_t g{0};
    uint16_t b{0};
};

class RgbImage {
public:
    RgbImage() = default;
    RgbImage(int width, int height, int bit_depth = 12);

    int width() const { return width_; }
    int height() const { return height_; }
    int bit_depth() const { return bit_depth_; }
    std::size_t size() const { return data_.size(); }
    uint16_t max_value() const;

    std::vector<Pixel>& data() { return data_; }
    const std::vector<Pixel>& data() const { return data_; }

    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;

private:
    int width_{0};
    int height_{0};
    int bit_depth_{12};
    std::vector<Pixel> data_;
};

} // namespace isp

#endif