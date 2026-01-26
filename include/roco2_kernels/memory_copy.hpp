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
    memory_copy()
    {

        mem_buffer = static_cast<uint64_t*>(malloc(mem_size * sizeof(uint64_t)));
        for (std::size_t i = 0; i < mem_size; i++)
        {
            mem_buffer[i] = i * 23 + 42;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        uint64_t loops;
        do
        {
            for (std::size_t i = 0; i < mem_size; i++)
            {
                mem_buffer[i] += mem_buffer[i];
            }
            loops++;
        } while (std::chrono::high_resolution_clock::now() < until);

        iteration_count_ = loops;
    }

    ~memory_copy()
    {
        free(mem_buffer);
    }

private:
    uint64_t* mem_buffer;
    static const std::size_t mem_size = 1024 * 1024;
};
} // namespace kernels
} // namespace roco2
