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
    sqrt() : vec_A(1024), vec_F(1024)
    {
        for (std::size_t i = 0; i < vec_A.size(); ++i)
        {
            vec_A[i] = static_cast<double>(i) * 0.3;
            vec_F[i] = static_cast<float>(i) * 1.42f;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        do
        {
            switch (type)
            {
            case 0:
                sqrtss_kernel(vec_F.data(), vec_F.size(), repeat);
                break;
            case 1:
                sqrtps_kernel(vec_F.data(), vec_F.size(), repeat);
                break;
            case 2:
                sqrtsd_kernel(vec_A.data(), vec_A.size(), repeat);
                break;
            case 3:
                sqrtpd_kernel(vec_A.data(), vec_A.size(), repeat);
                break;
            }
        } while (std::chrono::high_resolution_clock::now() < until);
    }

private:
    static const std::size_t repeat = 1024; // 4096;
    static const std::size_t type = 2;
    std::vector<double> vec_A;
    std::vector<float> vec_F;
};
} // namespace kernels
} // namespace roco2
