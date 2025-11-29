#include "modules/gamma.hpp"
#include <cmath>
#include <algorithm>

namespace isp {

void apply_gamma(RgbImage& img, double gamma) {
    if (img.size() == 0) return;
    if (gamma <= 0) return;

    double max_val = static_cast<double>(img.max_value());
    double inv_gamma = 1.0 / gamma;

    for (auto& p : img.data()) {
        double r = std::pow(p.r / max_val, inv_gamma) * max_val;
        double g = std::pow(p.g / max_val, inv_gamma) * max_val;
        double b = std::pow(p.b / max_val, inv_gamma) * max_val;

        p.r = static_cast<uint16_t>(std::min(r, max_val));
        p.g = static_cast<uint16_t>(std::min(g, max_val));
        p.b = static_cast<uint16_t>(std::min(b, max_val));
    }
}

} // namespace isp