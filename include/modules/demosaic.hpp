#ifndef ISP_PIPELINE_MODULES_DEMOSAIC_HPP
#define ISP_PIPELINE_MODULES_DEMOSAIC_HPP

#include "image.hpp"
#include "rgb_image.hpp"

namespace isp {

RgbImage demosaic(const Image& raw);

} // namespace isp

#endif