#include "modules/sharpen.hpp"
#include <algorithm>
#include <omp.h>

namespace isp {

void apply_sharpen(RgbImage& img) {
    if (img.width() < 3 || img.height() < 3) return;

    const int w = img.width();
    const int h = img.height();
    const uint16_t max_val = img.max_value();

    // Make a copy for reading (convolution needs original values)
    std::vector<Pixel> original = img.data();

    auto get = [&](int x, int y) -> const Pixel& {
        x = std::max(0, std::min(x, w - 1));
        y = std::max(0, std::min(y, h - 1));
        return original[static_cast<std::size_t>(y * w + x)];
    };

    // Sharpening kernel:
    //   0  -1   0
    //  -1   5  -1
    //   0  -1   0
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const Pixel& center = get(x, y);
            const Pixel& top    = get(x, y - 1);
            const Pixel& bottom = get(x, y + 1);
            const Pixel& left   = get(x - 1, y);
            const Pixel& right  = get(x + 1, y);

            auto clamp = [max_val](int val) -> uint16_t {
                return static_cast<uint16_t>(std::max(0, std::min(val, static_cast<int>(max_val))));
            };

            int r = 5 * center.r - top.r - bottom.r - left.r - right.r;
            int g = 5 * center.g - top.g - bottom.g - left.g - right.g;
            int b = 5 * center.b - top.b - bottom.b - left.b - right.b;

            Pixel& out = img.at(x, y);
            out.r = clamp(r);
            out.g = clamp(g);
            out.b = clamp(b);
        }
    }
}

} // namespace isp