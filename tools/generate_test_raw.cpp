#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>

int main() {
    const int width = 640;
    const int height = 480;
    const uint16_t R_val = 3000;
    const uint16_t G_val = 2000;
    const uint16_t B_val = 1000;

    std::vector<uint16_t> data(width * height);

    // Fill with RGGB pattern
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool even_row = (y % 2 == 0);
            bool even_col = (x % 2 == 0);

            uint16_t val;
            if (even_row && even_col)       val = R_val;  // R
            else if (even_row && !even_col) val = G_val;  // G (on R row)
            else if (!even_row && even_col) val = G_val;  // G (on B row)
            else                            val = B_val;  // B

            data[static_cast<std::size_t>(y * width + x)] = val;
        }
    }

    // Write as little-endian 16-bit raw
    std::ofstream file("data/test.raw", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file\n";
        return 1;
    }

    for (uint16_t pixel : data) {
        uint8_t low = pixel & 0xFF;
        uint8_t high = (pixel >> 8) & 0xFF;
        file.put(static_cast<char>(low));
        file.put(static_cast<char>(high));
    }

    std::cout << "Generated: data/test.raw (" << width << "x" << height << ", 12-bit)\n";
    return 0;
}