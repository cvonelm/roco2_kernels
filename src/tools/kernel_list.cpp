// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/base_kernel.hpp>

#include <iostream>

int main(void)
{
    for (auto kernel : roco2::kernels::kernel_names())
    {
        std::cout << kernel << std::endl;
    }
}
