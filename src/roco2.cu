// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/gpu.hpp>

#include <cuda_runtime.h>

#include <fmt/core.h>

#include <chrono>
#include <stdexcept>

namespace roco2
{
namespace kernels
{
// TODO This does not yet reach the full TDP of an NVidia GraceHopper
// device which can be definitly be reached with GPU applications only
//
// Maybe FP16 to the rescue?
//
// TODO This whole kernel is still a bit "wobbly" in its power consumption.
// Find a way to make the power consumption more stable.
__global__ void load_kernel(float* x, int nsize)
{
    int tid = threadIdx.x + blockDim.x * blockIdx.x;

    for (int a = tid; a < nsize; a += blockDim.x * gridDim.x)
    {
#pragma unroll
        for (int i = 0; i < 100; i++)
        {
            x[a] *= 2;
            x[a] += 2;
            x[a] /= 2;
            x[a] -= 1;
        }
    }
}

// Kernel rationale:
// - If we schedule less blocks than there are SMs available,
//   then we can control what percentage of all SMs is on at
//   any given point (scheduled_blocks/total SMs)
void run_kernel(std::chrono::high_resolution_clock::time_point until, int percentage)
{

    cudaError_t ret;
    cudaDeviceProp devProp;
    if ((ret = cudaGetDeviceProperties(&devProp, 0)) != cudaSuccess)
    {
        throw std::runtime_error(
            fmt::format("Failed to get GPU device properties: {}", cudaGetErrorString(ret)));
    }

    int nsize = devProp.multiProcessorCount * devProp.maxThreadsPerBlock * 3000;

    float* d_x;
    float* h_x = (float*)malloc(nsize * sizeof(float));

    if ((ret = cudaMalloc((void**)&d_x, nsize * sizeof(float))) != cudaSuccess)
    {
        throw std::runtime_error(
            fmt::format("Failed to alloc on GPU: {}", cudaGetErrorString(ret)));
    }

    for (int foobar = 0; foobar < nsize; foobar++)
    {
        h_x[foobar] = 1.0;
    }

    cudaMemcpy(d_x, h_x, nsize * sizeof(float), cudaMemcpyHostToDevice);
    int nthreads = 1024;

    int nblocks = ceil(static_cast<double>(devProp.multiProcessorCount * percentage) / 100);

    while (std::chrono::high_resolution_clock::now() < until)
    {
        load_kernel<<<nblocks, nthreads>>>(d_x, nsize);
        cudaDeviceSynchronize();
    }
}

} // namespace kernels
} // namespace roco2
