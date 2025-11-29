#include "modules/awb.hpp"
#include <algorithm>

namespace isp {

void apply_awb(RgbImage& img) {
    if (img.size() == 0) return;

    // Calculate channel averages
    double r_sum = 0, g_sum = 0, b_sum = 0;
    for (const auto& p : img.data()) {
        r_sum += p.r;
        g_sum += p.g;
        b_sum += p.b;
    }

    double count = static_cast<double>(img.size());
    double r_avg = r_sum / count;
    double g_avg = g_sum / count;
    double b_avg = b_sum / count;

    // Avoid division by zero
    if (r_avg < 1.0) r_avg = 1.0;
    if (g_avg < 1.0) g_avg = 1.0;
    if (b_avg < 1.0) b_avg = 1.0;

    // Use max average as reference (preserve brightness)
    double max_avg = std::max({r_avg, g_avg, b_avg});

    double r_gain = max_avg / r_avg;
    double g_gain = max_avg / g_avg;
    double b_gain = max_avg / b_avg;

    // Apply gains
    uint16_t max_val = img.max_value();
    for (auto& p : img.data()) {
        double new_r = p.r * r_gain;
        double new_g = p.g * g_gain;
        double new_b = p.b * b_gain;

        // Clamp to valid range
        p.r = static_cast<uint16_t>(std::min(new_r, static_cast<double>(max_val)));
        p.g = static_cast<uint16_t>(std::min(new_g, static_cast<double>(max_val)));
        p.b = static_cast<uint16_t>(std::min(new_b, static_cast<double>(max_val)));
    }
}

} // namespace isp