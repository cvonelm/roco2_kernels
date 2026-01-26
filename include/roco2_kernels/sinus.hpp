// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>
#include <cmath>

namespace roco2
{
namespace kernels
{

class sinus : public base_kernel
{
public:
    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        double m = 0.0;

        uint64_t loops;
        do
        {
            for (std::size_t i = 0; i < sinus_loop; i++)
            {
                m += sin((double)i);
            }
            loops++;
        } while (std::chrono::high_resolution_clock::now() < until);

        iteration_count_ = loops;
        // just as a data dependency
        volatile int dd = 0;
        if (m == 42.0)
            dd++;
    }

    static const std::size_t sinus_loop = 200000;
};
} // namespace kernels
} // namespace roco2
