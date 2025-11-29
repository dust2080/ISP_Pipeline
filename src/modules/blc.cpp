#include "modules/blc.hpp"

namespace isp {

void apply_blc(Image& img, uint16_t black_level) {
    for (auto& pixel : img.data()) {
        if (pixel > black_level) {
            pixel -= black_level;
        } else {
            pixel = 0;
        }
    }
}

} // namespace isp