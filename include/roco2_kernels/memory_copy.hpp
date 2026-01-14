// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>

namespace roco2
{
namespace kernels
{

class memory_copy : public base_kernel
{
public:
    memory_copy() : mem_buffer(mem_size)
    {
        for (std::size_t i = 0; i < mem_buffer.size(); i++)
        {
            mem_buffer[i] = i * 23 + 42;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        do
        {
            for (std::size_t i = 0; i < mem_buffer.size(); i++)
            {
                mem_buffer[i] += mem_buffer[i];
            }
        } while (std::chrono::high_resolution_clock::now() < until);
    }

private:
    std::vector<std::uint64_t> mem_buffer;
    static const std::size_t mem_size = 1024 * 1024;
};
} // namespace kernels
} // namespace roco2
