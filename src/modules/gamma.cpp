#include "modules/gamma.hpp"
#include <cmath>
#include <vector>

namespace isp {

void apply_gamma(RgbImage& img, double gamma) {
    if (img.size() == 0) return;
    if (gamma <= 0) return;

    uint16_t max_val = img.max_value();
    double inv_gamma = 1.0 / gamma;

    // Build LUT
    std::vector<uint16_t> lut(max_val + 1);
    for (int i = 0; i <= max_val; ++i) {
        double normalized = static_cast<double>(i) / max_val;
        double corrected = std::pow(normalized, inv_gamma);
        lut[static_cast<std::size_t>(i)] = static_cast<uint16_t>(corrected * max_val);
    }

    // Apply LUT
    for (auto& p : img.data()) {
        p.r = lut[p.r];
        p.g = lut[p.g];
        p.b = lut[p.b];
    }
}

} // namespace isp