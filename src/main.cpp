#include "image.hpp"
#include "io.hpp"
#include "rgb_image.hpp"
#include "modules/blc.hpp"
#include "modules/demosaic.hpp"
#include "modules/awb.hpp"
#include "modules/gamma.hpp"
#include "modules/sharpen.hpp"
#include <iostream>

int main() {
    // 1. Load RAW
    isp::RawFileConfig config;
    config.width = 640;
    config.height = 480;
    config.bit_depth = 12;
    config.pattern = isp::BayerPattern::RGGB;
    config.little_endian = true;

    std::cout << "Loading RAW...\n";
    auto result = isp::load_raw("data/test.raw", config);
    if (!result) {
        std::cerr << "Failed to load RAW\n";
        return 1;
    }
    isp::Image& raw = *result;
    std::cout << "Loaded: " << raw.width() << "x" << raw.height() << "\n";

    // 2. BLC
    std::cout << "Applying BLC...\n";
    isp::apply_blc(raw, 64);

    // 3. Demosaic
    std::cout << "Applying Demosaic...\n";
    isp::RgbImage rgb = isp::demosaic(raw);

    // 4. AWB
    std::cout << "Applying AWB...\n";
    isp::apply_awb(rgb);

    // 5. Gamma
    std::cout << "Applying Gamma...\n";
    isp::apply_gamma(rgb, 2.2);

    // 6. Sharpen
    std::cout << "Applying Sharpen...\n";
    isp::apply_sharpen(rgb);

    // 7. Save output
    std::cout << "Saving output...\n";
    if (isp::save_ppm("data/output.ppm", rgb)) {
        std::cout << "Saved: data/output.ppm\n";
    }

    std::cout << "Pipeline complete!\n";
    return 0;
}