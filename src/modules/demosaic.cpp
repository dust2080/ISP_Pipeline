#include "modules/demosaic.hpp"

namespace isp {

namespace {

// Safe pixel access with boundary clamping
uint16_t get_pixel(const Image& img, int x, int y) {
    x = std::max(0, std::min(x, img.width() - 1));
    y = std::max(0, std::min(y, img.height() - 1));
    return img.data()[static_cast<std::size_t>(y * img.width() + x)];
}

} // anonymous namespace

RgbImage demosaic(const Image& raw) {
    if (raw.pattern() != BayerPattern::RGGB) {
        throw std::runtime_error("Only RGGB pattern is supported");
    }

    const int w = raw.width();
    const int h = raw.height();
    RgbImage rgb(w, h, raw.bit_depth());

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            bool even_row = (y % 2 == 0);
            bool even_col = (x % 2 == 0);

            Pixel p;
            uint16_t center = get_pixel(raw, x, y);

            if (even_row && even_col) {
                // R pixel
                p.r = center;
                p.g = static_cast<uint16_t>((
                    get_pixel(raw, x-1, y) + get_pixel(raw, x+1, y) +
                    get_pixel(raw, x, y-1) + get_pixel(raw, x, y+1)) / 4);
                p.b = static_cast<uint16_t>((
                    get_pixel(raw, x-1, y-1) + get_pixel(raw, x+1, y-1) +
                    get_pixel(raw, x-1, y+1) + get_pixel(raw, x+1, y+1)) / 4);
            }
            else if (even_row && !even_col) {
                // G pixel on R row
                p.g = center;
                p.r = static_cast<uint16_t>((
                    get_pixel(raw, x-1, y) + get_pixel(raw, x+1, y)) / 2);
                p.b = static_cast<uint16_t>((
                    get_pixel(raw, x, y-1) + get_pixel(raw, x, y+1)) / 2);
            }
            else if (!even_row && even_col) {
                // G pixel on B row
                p.g = center;
                p.r = static_cast<uint16_t>((
                    get_pixel(raw, x, y-1) + get_pixel(raw, x, y+1)) / 2);
                p.b = static_cast<uint16_t>((
                    get_pixel(raw, x-1, y) + get_pixel(raw, x+1, y)) / 2);
            }
            else {
                // B pixel
                p.b = center;
                p.g = static_cast<uint16_t>((
                    get_pixel(raw, x-1, y) + get_pixel(raw, x+1, y) +
                    get_pixel(raw, x, y-1) + get_pixel(raw, x, y+1)) / 4);
                p.r = static_cast<uint16_t>((
                    get_pixel(raw, x-1, y-1) + get_pixel(raw, x+1, y-1) +
                    get_pixel(raw, x-1, y+1) + get_pixel(raw, x+1, y+1)) / 4);
            }

            rgb.at(x, y) = p;
        }
    }

    return rgb;
}

} // namespace isp