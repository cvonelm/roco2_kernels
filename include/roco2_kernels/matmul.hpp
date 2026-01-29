// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

#include <chrono>

extern "C"
{
#include <cblas.h>
}

namespace roco2
{
namespace kernels
{

class matmul : public base_kernel
{
public:
    matmul()
    {
        mat_A = static_cast<double*>(malloc(mat_size * sizeof(double)));
        mat_B = static_cast<double*>(malloc(mat_size * sizeof(double)));
        mat_C = static_cast<double*>(malloc(mat_size * sizeof(double)));

        for (std::size_t i = 0; i < mat_size; i++)
        {
            mat_A[i] = static_cast<double>(i + 1);
            mat_B[i] = static_cast<double>(i + 1);
            mat_C[i] = static_cast<double>(i + 1);
        }
    }

    virtual void run(std::chrono::high_resolution_clock::time_point until) override
    {
        uint64_t m = 512;

        uint64_t loops = 0;
        do
        {
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1.0, mat_A, m, mat_B, m,
                        1.0, mat_C, m);
            loops++;
        } while (std::chrono::high_resolution_clock::now() < until);

        iteration_count_ = loops;
    }

    ~matmul()
    {
        free(mat_A);
        free(mat_B);
        free(mat_C);
    }

private:
    double* mat_A;
    double* mat_B;
    double* mat_C;
    uint64_t mat_size = 512 * 512;
};

} // namespace kernels
} // namespace roco2
