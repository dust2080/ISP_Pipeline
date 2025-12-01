# ISP Pipeline

A simple Image Signal Processor (ISP) pipeline simulator written in Modern C++ (C++20).

## Overview

This project simulates the core image processing steps that happen inside a camera's ISP, converting RAW sensor data into a viewable RGB image.

## Pipeline
```
RAW → BLC → Demosaic → AWB → Gamma → Sharpen → RGB Output
```

## Modules

| Module | Description | Algorithm |
|--------|-------------|-----------|
| **BLC** | Black Level Correction | Subtract black level offset from raw data |
| **Demosaic** | Bayer to RGB conversion | Bilinear interpolation |
| **AWB** | Auto White Balance | Gray World algorithm |
| **Gamma** | Gamma correction | LUT-based, γ=2.2 |
| **Sharpen** | Edge enhancement | 3x3 convolution kernel |

## Results

| Input | Output (After ISP) |
|-------|-------------------|
| ![input](docs/real_input.png) | ![output](docs/real_output.png) |

## Performance Benchmark

Test image: 1206 x 2144 (2.5 MP)  
Platform: MacBook Air M1 / Intel i7 (填你的機器)

| Module | Time | Percentage |
|--------|------|------------|
| BLC | 32 ms | 3% |
| Demosaic | 466 ms | 50% |
| AWB | 75 ms | 8% |
| Gamma | 27 ms | 3% |
| Sharpen | 332 ms | 36% |
| **Total** | **932 ms** | 100% |

### Analysis

- **Demosaic** and **Sharpen** dominate the pipeline (86% of total time)
- Both require neighbor pixel access, causing more cache misses
- **Gamma** is fast due to LUT optimization

### Potential Optimizations

- Multi-threading with OpenMP (expected 3-4x speedup)
- SIMD instructions (AVX2) for parallel pixel processing
- Cache-friendly tiling to improve memory access patterns
- Reduce memory copy in Sharpen module

## Project Structure
```
ISP_Pipeline/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── image.hpp          # RAW image container
│   ├── rgb_image.hpp      # RGB image container
│   ├── io.hpp             # File I/O (RAW, PNG, PPM)
│   └── modules/
│       ├── blc.hpp
│       ├── demosaic.hpp
│       ├── awb.hpp
│       ├── gamma.hpp
│       └── sharpen.hpp
├── src/
│   ├── main.cpp
│   ├── image.cpp
│   ├── rgb_image.cpp
│   ├── io.cpp
│   └── modules/
│       ├── blc.cpp
│       ├── demosaic.cpp
│       ├── awb.cpp
│       ├── gamma.cpp
│       └── sharpen.cpp
├── tools/
│   └── generate_test_raw.cpp
└── vendor/
    ├── stb_image.h
    └── stb_image_write.h
```

## Build

### Requirements
- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20+

### Steps
```bash
git clone https://github.com/dust2080/ISP_Pipeline.git
cd ISP_Pipeline
mkdir build && cd build
cmake ..
make
```

## Usage

### Using test RAW file
```bash
./build/generate_test_raw    # Generate test RAW
./build/isp_main             # Run pipeline
```

### Using PNG input
```bash
./build/isp_main path/to/image.png
```

Output will be saved to `data/output.png`.

## Technical Details

### Why Bilinear Demosaic?
Bilinear interpolation is the simplest demosaicing algorithm. While more advanced algorithms (e.g., AHD, VNG) produce better edge quality, bilinear is sufficient to demonstrate the ISP concept and is easier to understand.

### Why Gray World AWB?
Gray World assumes the average color of a scene is neutral gray. It's simple and effective for most scenes. More sophisticated methods like illuminant estimation could be added for better accuracy.

### Why LUT for Gamma?
Computing `pow()` for every pixel is expensive. A Lookup Table (LUT) pre-computes all 4096 possible values (for 12-bit), reducing gamma correction to simple array lookups.

## Future Improvements

- [ ] Support more Bayer patterns (BGGR, GRBG, GBRG)
- [ ] Add noise reduction module
- [ ] Implement edge-directed demosaicing
- [ ] Support real RAW formats (DNG) via libraw
- [ ] Add performance benchmarks

## License

MIT License