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
#include <chrono>

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
    std::cout << "Loaded: " << raw.width() << "x" << raw.height() << "\n\n";

    // Benchmark helper
    using Clock = std::chrono::high_resolution_clock;
    auto total_start = Clock::now();

    std::cout << "=== Pipeline Benchmark ===\n";

    // BLC
    auto start = Clock::now();
    isp::apply_blc(raw, 64);
    auto end = Clock::now();
    auto blc_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "BLC:      " << blc_time << " us\n";

    // Demosaic
    start = Clock::now();
    isp::RgbImage rgb = isp::demosaic(raw);
    end = Clock::now();
    auto demosaic_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Demosaic: " << demosaic_time << " us\n";

    // AWB
    start = Clock::now();
    isp::apply_awb(rgb);
    end = Clock::now();
    auto awb_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "AWB:      " << awb_time << " us\n";

    // Gamma
    start = Clock::now();
    isp::apply_gamma(rgb, 2.2);
    end = Clock::now();
    auto gamma_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Gamma:    " << gamma_time << " us\n";

    // Sharpen
    start = Clock::now();
    isp::apply_sharpen(rgb);
    end = Clock::now();
    auto sharpen_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Sharpen:  " << sharpen_time << " us\n";

    auto total_end = Clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::microseconds>(total_end - total_start).count();
    std::cout << "-------------------------\n";
    std::cout << "Total:    " << total_time << " us\n\n";

    // Save
    std::cout << "Saving output...\n";
    isp::save_ppm("data/output.ppm", rgb);
    isp::save_png("data/output.png", rgb);
    std::cout << "Saved: data/output.png\n";

    return 0;
}