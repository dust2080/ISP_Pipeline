#include "image.hpp"
#include <iostream>

int main() {
    isp::Image img(1920, 1080, 12, isp::BayerPattern::RGGB);
    
    std::cout << "Image: " << img.width() << "x" << img.height() 
              << " @ " << img.bit_depth() << "-bit\n";
    
    return 0;
}