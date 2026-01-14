// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>

namespace roco2
{
namespace kernels
{

class busy_wait : public base_kernel
{
public:
    void run(std::chrono::high_resolution_clock::time_point tp)
    {
        std::size_t loops = 0;

        while (std::chrono::high_resolution_clock::now() < tp)
        {
            loops++;
        }

        (void)loops;
    }
};
} // namespace kernels
} // namespace roco2
