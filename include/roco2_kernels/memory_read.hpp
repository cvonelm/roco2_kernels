#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>
#include <cstddef>
#include <vector>

namespace roco2
{
namespace kernels
{

class memory_read : public base_kernel
{
public:
    memory_read() : mem_buffer(mem_size)
    {
        for (std::size_t i = 0; i < mem_buffer.size(); i++)
        {
            mem_buffer[i] = i * 23 + 42;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        uint64_t m = 0;
        do
        {
            for (std::size_t i = 0; i < mem_buffer.size(); i++)
            {
                m += mem_buffer[i];
            }
        } while (std::chrono::high_resolution_clock::now() < until);

        // just as a data dependency
        volatile int dd = 0;
        if (m == 42)
            dd++;
    }

private:
    std::vector<std::uint64_t> mem_buffer;
    static const std::size_t mem_size = 1024 * 1024;
};
} // namespace kernels
} // namespace roco2
