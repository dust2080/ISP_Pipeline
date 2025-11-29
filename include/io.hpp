#ifndef ISP_PIPELINE_IO_HPP
#define ISP_PIPELINE_IO_HPP

#include "image.hpp"
#include "rgb_image.hpp"
#include <string>
#include <optional>

namespace isp {

struct RawFileConfig {
    int width;
    int height;
    int bit_depth = 12;
    BayerPattern pattern = BayerPattern::RGGB;
    bool little_endian = true;
};

std::optional<Image> load_raw(const std::string& path, const RawFileConfig& config);

// Grayscale output
bool save_ppm(const std::string& path, const Image& img);

// RGB output
bool save_ppm(const std::string& path, const RgbImage& img);

} // namespace isp

#endif