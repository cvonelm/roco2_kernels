// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

namespace roco2
{
namespace kernels
{

class compute : public base_kernel
{
public:
    compute() : vec_A(1024), vec_B(1024), vec_C(1024)
    {
        for (std::size_t i = 0; i < vec_A.size(); ++i)
        {
            vec_A[i] = static_cast<double>(i) * 0.3;
            vec_B[i] = static_cast<double>(i) * 0.2;
            vec_C[i] = static_cast<double>(i) * 0.7;
        }
    }

    virtual void run(std::chrono::high_resolution_clock::time_point until) override
    {
        std::size_t loops = 0;

        while (std::chrono::high_resolution_clock::now() <= until)
        {
            if (vec_C[0] == 123.12345)
                vec_A[0] += 1.0;

            compute_kernel(vec_A, vec_B, vec_C, 32);

            loops++;
        }

        (void)loops;

        // just as a data dependency
        volatile int dd = 0;
        if (vec_C[0] == 42.0)
            dd++;
    }

private:
    void compute_kernel(std::vector<double>& A, std::vector<double>& B, std::vector<double>& C,
                        std::size_t repeat)
    {
        double m = C[0];
        const auto size = 1024;

        for (std::size_t i = 0; i < repeat; i++)
        {
            for (uint64_t i = 0; i < size; i++)
            {
                m += B[i] * A[i];
            }
            C[0] = m;
        }
    }

private:
    std::vector<double> vec_A;
    std::vector<double> vec_B;
    std::vector<double> vec_C;
};
} // namespace kernels
} // namespace roco2
