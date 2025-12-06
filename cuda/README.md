# CUDA Bilateral Filter

GPU-accelerated bilateral filter implementation using CUDA.

## Performance

| Platform | Time | Speedup |
|----------|------|---------|
| CPU (single-threaded) | ~20,000 ms | 1x |
| GPU (Tesla T4) | ~10 ms | **1930x** |

Test image: 1206 x 2144 x 3 (RGB)

## Build

Requires NVIDIA GPU and CUDA toolkit.
```bash
nvcc -arch=sm_75 -o bilateral_cuda bilateral_cuda.cu
./bilateral_cuda
```

Note: Change `-arch=sm_75` based on your GPU:
- Tesla T4: sm_75
- RTX 3090: sm_86
- RTX 4090: sm_89

## Testing on Google Colab

If you don't have an NVIDIA GPU locally, you can test on Google Colab:

1. Go to [Google Colab](https://colab.research.google.com/)
2. Runtime → Change runtime type → T4 GPU
3. Upload `bilateral_cuda.cu`
4. Run:
```python
!nvcc -arch=sm_75 -o bilateral_cuda bilateral_cuda.cu
!./bilateral_cuda
```

## How It Works

The CUDA implementation parallelizes the bilateral filter by assigning one thread per pixel:
```
CPU: Process pixels one by one (sequential)
GPU: Process all pixels simultaneously (parallel)

Grid (76 x 134 blocks)
└── Block (16 x 16 = 256 threads)
    └── Thread → processes 1 pixel
```

Total threads: ~2.6 million (one per pixel)