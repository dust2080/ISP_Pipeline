#ifndef ISP_PIPELINE_IMAGE_HPP
#define ISP_PIPELINE_IMAGE_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>

namespace isp {

enum class BayerPattern { RGGB, BGGR, GRBG, GBRG };

class Image {
public:
    Image() = default;
    Image(int width, int height, int bit_depth = 12, 
          BayerPattern pattern = BayerPattern::RGGB);

    int width() const { return width_; }
    int height() const { return height_; }
    int bit_depth() const { return bit_depth_; }
    BayerPattern pattern() const { return pattern_; }
    std::size_t size() const { return data_.size(); }
    
    // max pixel value based on bit depth (e.g., 4095 for 12-bit)
    uint16_t max_value() const;

    std::vector<uint16_t>& data() { return data_; }
    const std::vector<uint16_t>& data() const { return data_; }

    uint16_t& at(int x, int y);
    const uint16_t& at(int x, int y) const;

    void fill(uint16_t value);

private:
    int width_{0};
    int height_{0};
    int bit_depth_{12};
    BayerPattern pattern_{BayerPattern::RGGB};
    std::vector<uint16_t> data_;
};

} // namespace isp

#endif