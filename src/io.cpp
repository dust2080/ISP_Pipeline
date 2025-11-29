#include "io.hpp"
#include <fstream>
#include <iostream>

namespace isp {

std::optional<Image> load_raw(const std::string& path, const RawFileConfig& config) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open: " << path << '\n';
        return std::nullopt;
    }

    Image img(config.width, config.height, config.bit_depth, config.pattern);
    auto& data = img.data();
    const bool is_16bit = config.bit_depth > 8;

    if (is_16bit) {
        // 16-bit: read two bytes per pixel
        std::vector<uint8_t> buffer(data.size() * 2);
        file.read(reinterpret_cast<char*>(buffer.data()), 
                  static_cast<std::streamsize>(buffer.size()));

        for (std::size_t i = 0; i < data.size(); ++i) {
            uint16_t low = buffer[i * 2];
            uint16_t high = buffer[i * 2 + 1];
            data[i] = static_cast<uint16_t>(config.little_endian ? (high << 8 | low) : (low << 8 | high));
        }
    } else {
        // 8-bit: one byte per pixel
        std::vector<uint8_t> buffer(data.size());
        file.read(reinterpret_cast<char*>(buffer.data()), 
                  static_cast<std::streamsize>(buffer.size()));

        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i] = buffer[i];
        }
    }

    return img;
}

bool save_ppm(const std::string& path, const Image& img) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create: " << path << '\n';
        return false;
    }

    const int w = img.width();
    const int h = img.height();
    const uint16_t max_val = img.max_value();
    const auto& data = img.data();

    // PPM header (P5 = grayscale binary)
    file << "P5\n" << w << " " << h << "\n" << max_val << "\n";

    if (max_val > 255) {
        // 16-bit: PPM uses big-endian
        for (std::size_t i = 0; i < data.size(); ++i) {
            uint8_t high = static_cast<uint8_t>(data[i] >> 8);
            uint8_t low = static_cast<uint8_t>(data[i] & 0xFF);
            file.put(static_cast<char>(high));
            file.put(static_cast<char>(low));
        }
    } else {
        for (std::size_t i = 0; i < data.size(); ++i) {
            file.put(static_cast<char>(data[i]));
        }
    }

    return true;
}

} // namespace isp