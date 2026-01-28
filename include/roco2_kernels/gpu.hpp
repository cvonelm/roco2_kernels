// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>
#include <stdexcept>

namespace roco2
{
namespace kernels
{
void run_kernel(std::chrono::high_resolution_clock::time_point until, int percentage);

class gpu_kernel : base_kernel
{
public:
    // Percentage of SMs which will be used
    gpu_kernel(int percentage) : percentage_(percentage)
    {
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        run_kernel(until, percentage_);
    }

private:
    int percentage_;
};

#ifndef HAVE_CUDA
inline void run_kernel(std::chrono::high_resolution_clock::time_point until, int percentage)
{
    throw std::runtime_error("roco2_kernels was built without CUDA support!");
}
#endif

} // namespace kernels

} // namespace roco2
