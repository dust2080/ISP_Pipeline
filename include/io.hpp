#ifndef ISP_PIPELINE_IO_HPP
#define ISP_PIPELINE_IO_HPP

#include "image.hpp"
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

// Load raw sensor data from binary file
std::optional<Image> load_raw(const std::string& path, const RawFileConfig& config);

// Save image as grayscale PPM (simple format, no external lib needed)
bool save_ppm(const std::string& path, const Image& img);

} // namespace isp

#endif