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
    matmul() : mat_A(512 * 512), mat_B(512 * 512), mat_C(512 * 512)
    {
        for (std::size_t i = 0; i < mat_A.size(); i++)
        {
            mat_A[i] = static_cast<double>(i + 1);
            mat_B[i] = static_cast<double>(i + 1);
            mat_C[i] = static_cast<double>(i + 1);
        }
    }

    virtual void run(std::chrono::high_resolution_clock::time_point until) override
    {
        double* A = mat_A.data();
        double* B = mat_B.data();
        double* C = mat_C.data();

        uint64_t m = 512;

        do
        {
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1.0, A, m, B, m, 1.0, C,
                        m);
        } while (std::chrono::high_resolution_clock::now() < until);
    }

private:
    std::vector<double> mat_A;
    std::vector<double> mat_B;
    std::vector<double> mat_C;
};

} // namespace kernels
} // namespace roco2
