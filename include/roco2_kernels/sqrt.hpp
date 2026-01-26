// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/asm_kernels.hpp>
#include <roco2_kernels/base_kernel.hpp>

#include <chrono>

namespace roco2
{
namespace kernels
{
class sqrt : public base_kernel
{
public:
    sqrt()
    {
        vec_A = static_cast<double*>(malloc(vec_size * sizeof(double)));
        vec_F = static_cast<float*>(malloc(vec_size * sizeof(double)));
        for (std::size_t i = 0; i < vec_size; ++i)
        {
            vec_A[i] = static_cast<double>(i) * 0.3;
            vec_F[i] = static_cast<float>(i) * 1.42f;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        uint64_t loops;
        do
        {
            switch (type)
            {
            case 0:
                sqrtss_kernel(vec_F, vec_size, repeat);
                break;
            case 1:
                sqrtps_kernel(vec_F, vec_size, repeat);
                break;
            case 2:
                sqrtsd_kernel(vec_A, vec_size, repeat);
                break;
            case 3:
                sqrtpd_kernel(vec_A, vec_size, repeat);
                break;
            }
            loops++;
        } while (std::chrono::high_resolution_clock::now() < until);
        iteration_count_ = loops;
    }

    ~sqrt()
    {
        free(vec_A);
        free(vec_F);
    }

private:
    static const std::size_t repeat = 1024; // 4096;
    static const std::size_t type = 2;
    static const std::size_t vec_size = 1024;
    double* vec_A;
    float* vec_F;
};
} // namespace kernels
} // namespace roco2
