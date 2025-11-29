#include "image.hpp"
#include "io.hpp"
#include <iostream>

int main() {
    isp::RawFileConfig config;
    config.width = 640;
    config.height = 480;
    config.bit_depth = 12;
    config.pattern = isp::BayerPattern::RGGB;
    config.little_endian = true;

    auto result = isp::load_raw("data/test.raw", config);
    if (!result) {
        std::cerr << "Failed to load RAW\n";
        return 1;
    }

    isp::Image& img = *result;
    std::cout << "Loaded: " << img.width() << "x" << img.height() 
              << " @ " << img.bit_depth() << "-bit\n";

    if (isp::save_ppm("data/output.ppm", img)) {
        std::cout << "Saved: data/output.ppm\n";
    }

    return 0;
}