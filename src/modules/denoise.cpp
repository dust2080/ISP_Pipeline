#include "modules/denoise.hpp"
#include <cmath>
#include <algorithm>
#include <omp.h>

namespace isp {

void apply_denoise(RgbImage& img, float sigma_spatial, float sigma_range) {
    const int w = img.width();
    const int h = img.height();
    const uint16_t max_val = img.max_value();
    
    // Kernel radius based on sigma_spatial
    const int radius = static_cast<int>(std::ceil(2.0f * sigma_spatial));
    
    // Pre-compute spatial weights
    const float spatial_coeff = -0.5f / (sigma_spatial * sigma_spatial);
    const float range_coeff = -0.5f / (sigma_range * sigma_range);
    
    // Make a copy for reading
    std::vector<Pixel> original = img.data();
    
    auto get = [&](int x, int y) -> const Pixel& {
        x = std::max(0, std::min(x, w - 1));
        y = std::max(0, std::min(y, h - 1));
        return original[static_cast<std::size_t>(y * w + x)];
    };
    
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const Pixel& center = get(x, y);
            
            float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;
            float sum_weight = 0.0f;
            
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    const Pixel& neighbor = get(x + dx, y + dy);
                    
                    // Spatial weight
                    float spatial_dist = static_cast<float>(dx * dx + dy * dy);
                    float spatial_weight = std::exp(spatial_dist * spatial_coeff);
                    
                    // Range weight (color similarity)
                    float dr = static_cast<float>(neighbor.r) - static_cast<float>(center.r);
                    float dg = static_cast<float>(neighbor.g) - static_cast<float>(center.g);
                    float db = static_cast<float>(neighbor.b) - static_cast<float>(center.b);
                    float color_dist = dr * dr + dg * dg + db * db;
                    float range_weight = std::exp(color_dist * range_coeff);
                    
                    // Combined weight
                    float weight = spatial_weight * range_weight;
                    
                    sum_r += weight * static_cast<float>(neighbor.r);
                    sum_g += weight * static_cast<float>(neighbor.g);
                    sum_b += weight * static_cast<float>(neighbor.b);
                    sum_weight += weight;
                }
            }
            
            // Normalize
            Pixel& out = img.at(x, y);
            out.r = static_cast<uint16_t>(std::clamp(sum_r / sum_weight, 0.0f, static_cast<float>(max_val)));
            out.g = static_cast<uint16_t>(std::clamp(sum_g / sum_weight, 0.0f, static_cast<float>(max_val)));
            out.b = static_cast<uint16_t>(std::clamp(sum_b / sum_weight, 0.0f, static_cast<float>(max_val)));
        }
    }
}

} // namespace isp