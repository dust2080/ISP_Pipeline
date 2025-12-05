#ifndef ISP_DENOISE_HPP
#define ISP_DENOISE_HPP

#include "rgb_image.hpp"

namespace isp {

// Bilateral filter for noise reduction
// sigma_spatial: spatial kernel size (default: 2.0)
// sigma_range: color similarity threshold (default: 30.0)
void apply_denoise(RgbImage& img, float sigma_spatial = 2.0f, float sigma_range = 30.0f);

} // namespace isp

#endif // ISP_DENOISE_HPP