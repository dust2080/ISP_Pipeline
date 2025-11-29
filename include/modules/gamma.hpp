#ifndef ISP_PIPELINE_MODULES_GAMMA_HPP
#define ISP_PIPELINE_MODULES_GAMMA_HPP

#include "rgb_image.hpp"

namespace isp {

void apply_gamma(RgbImage& img, double gamma = 2.2);

} // namespace isp

#endif