// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <bowl/error.hpp>
#include <bowl/expected.hpp>

#include <chrono>
#include <memory>
#include <sys/types.h>
#include <vector>

namespace roco2
{
namespace kernels
{
enum class Kernel
{
    IDLE,
    ADDPD,
    MULPD,
    SINUS,
    BUSY_WAIT,
    COMPUTE,
    FIRESTARTER,
    BLAS,
    MEMORY_WRITE,
    MEMORY_COPY,
    MEMORY_READ,
    SQRT,
    _KERNEL_MAX
};

class base_kernel
{
public:
    virtual void run(std::chrono::high_resolution_clock::time_point until) = 0;

    virtual ~base_kernel()
    {
    }

    uint64_t iteration_count()
    {
        return iteration_count_;
    }

protected:
    uint64_t iteration_count_ = 0;

private:
};

bowl::Expected<Kernel, bowl::CustomError> str_to_kernel(std::string kernel);

bowl::Expected<std::unique_ptr<roco2::kernels::base_kernel>, bowl::CustomError>
kernel_from_str(std::string str);

std::unique_ptr<roco2::kernels::base_kernel> kernel_from_enum(Kernel k);

std::vector<std::string> kernel_names();

} // namespace kernels
} // namespace roco2
