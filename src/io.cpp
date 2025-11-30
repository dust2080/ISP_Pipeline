#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write.h"
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

std::optional<Image> load_png_as_raw(const std::string& path, BayerPattern pattern) {
    int w, h, channels;
    uint8_t* data = stbi_load(path.c_str(), &w, &h, &channels, 3); // Force RGB
    if (!data) {
        std::cerr << "Failed to load: " << path << '\n';
        return std::nullopt;
    }

    Image raw(w, h, 12, pattern);
    auto& raw_data = raw.data();

    // Simulate Bayer pattern: keep only one channel per pixel
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            std::size_t src_idx = static_cast<std::size_t>((y * w + x) * 3);
            std::size_t dst_idx = static_cast<std::size_t>(y * w + x);

            uint8_t r = data[src_idx + 0];
            uint8_t g = data[src_idx + 1];
            uint8_t b = data[src_idx + 2];

            bool even_row = (y % 2 == 0);
            bool even_col = (x % 2 == 0);

            uint8_t value;
            if (pattern == BayerPattern::RGGB) {
                if (even_row && even_col)       value = r;
                else if (even_row && !even_col) value = g;
                else if (!even_row && even_col) value = g;
                else                            value = b;
            } else {
                // Fallback: only RGGB supported
                value = g;
            }

            // Convert 8-bit to 12-bit
            raw_data[dst_idx] = static_cast<uint16_t>(value) << 4;
        }
    }

    stbi_image_free(data);
    return raw;
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

bool save_ppm(const std::string& path, const RgbImage& img) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create: " << path << '\n';
        return false;
    }

    const int w = img.width();
    const int h = img.height();
    const uint16_t max_val = img.max_value();
    const auto& data = img.data();

    // PPM header (P6 = RGB binary)
    file << "P6\n" << w << " " << h << "\n" << max_val << "\n";

    if (max_val > 255) {
        // 16-bit: PPM uses big-endian
        for (const auto& p : data) {
            file.put(static_cast<char>(p.r >> 8));
            file.put(static_cast<char>(p.r & 0xFF));
            file.put(static_cast<char>(p.g >> 8));
            file.put(static_cast<char>(p.g & 0xFF));
            file.put(static_cast<char>(p.b >> 8));
            file.put(static_cast<char>(p.b & 0xFF));
        }
    } else {
        for (const auto& p : data) {
            file.put(static_cast<char>(p.r));
            file.put(static_cast<char>(p.g));
            file.put(static_cast<char>(p.b));
        }
    }

    return true;
}

bool save_png(const std::string& path, const RgbImage& img) {
    const int w = img.width();
    const int h = img.height();
    const auto& data = img.data();
    const uint16_t max_val = img.max_value();

    // PNG 只支援 8-bit，需要轉換
    std::vector<uint8_t> buffer(static_cast<std::size_t>(w * h * 3));

    for (std::size_t i = 0; i < data.size(); ++i) {
        // 從原本的 bit depth 轉換到 8-bit
        buffer[i * 3 + 0] = static_cast<uint8_t>(data[i].r * 255 / max_val);
        buffer[i * 3 + 1] = static_cast<uint8_t>(data[i].g * 255 / max_val);
        buffer[i * 3 + 2] = static_cast<uint8_t>(data[i].b * 255 / max_val);
    }

    int result = stbi_write_png(path.c_str(), w, h, 3, buffer.data(), w * 3);
    return result != 0;
}

} // namespace isp