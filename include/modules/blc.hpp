#ifndef ISP_PIPELINE_MODULES_BLC_HPP
#define ISP_PIPELINE_MODULES_BLC_HPP

#include "image.hpp"

namespace isp {

void apply_blc(Image& img, uint16_t black_level);

} // namespace isp

#endif