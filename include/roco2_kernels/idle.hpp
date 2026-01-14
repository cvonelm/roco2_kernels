// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>
#include <thread>

namespace roco2
{
namespace kernels
{

class idle : public base_kernel
{
public:
    void run(std::chrono::high_resolution_clock::time_point tp) override
    {

        std::this_thread::sleep_until(tp);
    }

private:
};
} // namespace kernels
} // namespace roco2
