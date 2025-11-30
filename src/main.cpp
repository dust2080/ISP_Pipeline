#include "image.hpp"
#include "io.hpp"
#include "rgb_image.hpp"
#include "modules/blc.hpp"
#include "modules/demosaic.hpp"
#include "modules/awb.hpp"
#include "modules/gamma.hpp"
#include "modules/sharpen.hpp"
#include <iostream>
#include <optional>

int main(int argc, char* argv[]) {
    std::string input_path = "data/test.raw";
    bool use_png_input = false;

    if (argc > 1) {
        input_path = argv[1];
        if (input_path.size() > 4 && 
            input_path.substr(input_path.size() - 4) == ".png") {
            use_png_input = true;
        }
    }

    std::optional<isp::Image> result;

    if (use_png_input) {
        std::cout << "Loading PNG as RAW: " << input_path << "\n";
        result = isp::load_png_as_raw(input_path);
    } else {
        std::cout << "Loading RAW: " << input_path << "\n";
        isp::RawFileConfig config;
        config.width = 640;
        config.height = 480;
        config.bit_depth = 12;
        config.pattern = isp::BayerPattern::RGGB;
        config.little_endian = true;
        result = isp::load_raw(input_path, config);
    }

    if (!result) {
        std::cerr << "Failed to load image\n";
        return 1;
    }

    isp::Image raw = std::move(*result);
    std::cout << "Loaded: " << raw.width() << "x" << raw.height() << "\n";

    std::cout << "Applying BLC...\n";
    isp::apply_blc(raw, 64);

    std::cout << "Applying Demosaic...\n";
    isp::RgbImage rgb = isp::demosaic(raw);

    std::cout << "Applying AWB...\n";
    isp::apply_awb(rgb);

    std::cout << "Applying Gamma...\n";
    isp::apply_gamma(rgb, 2.2);

    std::cout << "Applying Sharpen...\n";
    isp::apply_sharpen(rgb);

    std::cout << "Saving output...\n";
    isp::save_ppm("data/output.ppm", rgb);
    isp::save_png("data/output.png", rgb);
    std::cout << "Saved: data/output.png\n";

    return 0;
}