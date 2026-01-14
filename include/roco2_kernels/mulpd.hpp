// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/asm_kernels.hpp>
#include <roco2_kernels/base_kernel.hpp>

#include <chrono>

namespace roco2
{
namespace kernels
{

class mulpd : public base_kernel
{
public:
    mulpd() : vec_A(1024)
    {
        for (std::size_t i = 0; i < vec_A.size(); ++i)
        {
            vec_A[i] = static_cast<double>(i) * 0.3;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        for (std::size_t i = 0; i < 16; ++i)
        {
            vec_A[i] = 1. + std::numeric_limits<double>::epsilon();
        }

        std::size_t loops = 0;

        do
        {
            mulpd_kernel(vec_A.data(), repeat);
            loops++;
        } while (std::chrono::high_resolution_clock::now() < until);

        (void)loops;
    }

    // should be around 2ms per call of mulpd_kernel
    static const std::size_t repeat = 4096 * 1024;

    std::vector<double> vec_A;
};
} // namespace kernels
} // namespace roco2
