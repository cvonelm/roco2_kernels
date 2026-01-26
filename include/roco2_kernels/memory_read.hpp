#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>
#include <cstddef>

namespace roco2
{
namespace kernels
{

class memory_read : public base_kernel
{
public:
    memory_read()
    {
        mem_buffer = static_cast<uint64_t*>(malloc(mem_size * sizeof(uint64_t)));
        for (std::size_t i = 0; i < mem_size; i++)
        {
            mem_buffer[i] = i * 23 + 42;
        }
    }

    void run(std::chrono::high_resolution_clock::time_point until) override
    {
        uint64_t m = 0;
        uint64_t loops;
        do
        {
            for (std::size_t i = 0; i < mem_size; i++)
            {

                m += mem_buffer[i];
            }
            loops++;
            side_effect = m;
        } while (std::chrono::high_resolution_clock::now() < until);

        iteration_count_ = loops;
    }

    ~memory_read()
    {
        free(mem_buffer);
    }

private:
    volatile int side_effect;
    std::uint64_t* mem_buffer;
    static const std::size_t mem_size = 4871 * 1024;
};
} // namespace kernels
} // namespace roco2
