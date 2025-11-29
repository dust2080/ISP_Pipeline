# ISP Pipeline

A simple Image Signal Processor (ISP) pipeline simulator written in Modern C++.

## Pipeline
```
RAW → BLC → Demosaic → AWB → Gamma → Sharpen → RGB Output
```

## Modules

- **BLC** - Black Level Correction
- **Demosaic** - Bilinear interpolation (RGGB Bayer pattern)
- **AWB** - Auto White Balance (Gray World algorithm)
- **Gamma** - Gamma correction with LUT optimization
- **Sharpen** - 3x3 convolution sharpening filter

## Build
```bash
mkdir build && cd build
cmake ..
make
```

## Run
```bash
# Generate test RAW
./build/generate_test_raw

# Run pipeline
./build/isp_main
```

Output: `data/output.ppm`

## Requirements

- C++20
- CMake 3.20+