// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/addpd.hpp>
#include <roco2_kernels/base_kernel.hpp>
#include <roco2_kernels/busy_wait.hpp>
#include <roco2_kernels/compute.hpp>
#include <roco2_kernels/firestarter.hpp>
#include <roco2_kernels/idle.hpp>
#include <roco2_kernels/matmul.hpp>
#include <roco2_kernels/memory_copy.hpp>
#include <roco2_kernels/memory_read.hpp>
#include <roco2_kernels/memory_write.hpp>
#include <roco2_kernels/mulpd.hpp>
#include <roco2_kernels/sinus.hpp>
#include <roco2_kernels/sqrt.hpp>

#include <bowl/error.hpp>
#include <bowl/expected.hpp>

#include <memory>
#include <stdexcept>

namespace roco2
{
namespace kernels
{

bowl::Expected<std::unique_ptr<roco2::kernels::base_kernel>, bowl::CustomError>
kernel_from_str(std::string str)
{
    auto kernel_res = str_to_kernel(str);

    if (!kernel_res.ok())
    {
        return bowl::Unexpected(kernel_res.unpack_error());
    }

    return kernel_from_enum(kernel_res.unpack_ok());
}

std::unique_ptr<roco2::kernels::base_kernel> kernel_from_enum(Kernel k)
{
    switch (k)
    {
    case Kernel::IDLE:
        return std::make_unique<roco2::kernels::idle>();
    case Kernel::ADDPD:
        return std::make_unique<roco2::kernels::addpd>();
    case Kernel::MULPD:
        return std::make_unique<roco2::kernels::mulpd>();
    case Kernel::SINUS:
        return std::make_unique<roco2::kernels::sinus>();

    case Kernel::BUSY_WAIT:
        return std::make_unique<roco2::kernels::busy_wait>();
    case Kernel::COMPUTE:
        return std::make_unique<roco2::kernels::compute>();
    case Kernel::FIRESTARTER:
        return std::make_unique<roco2::kernels::firestarter>();

    case Kernel::BLAS:
        return std::make_unique<roco2::kernels::matmul>();

    case Kernel::MEMORY_READ:
        return std::make_unique<roco2::kernels::memory_read>();
    case Kernel::MEMORY_COPY:
        return std::make_unique<roco2::kernels::memory_copy>();
    case Kernel::MEMORY_WRITE:
        return std::make_unique<roco2::kernels::memory_write>();
    case Kernel::SQRT:
        return std::make_unique<roco2::kernels::sqrt>();
    case Kernel::_KERNEL_MAX:
        throw std::runtime_error("kernel_max is not a real value!");
    }

    throw std::runtime_error("UNREACHABLE!");
}

static std::map<std::string, Kernel> str_to_kernel_map = { { "idle", Kernel::IDLE },
                                                           { "addpd", Kernel::ADDPD },
                                                           { "mulpd", Kernel::MULPD },
                                                           { "sinus", Kernel::SINUS },
                                                           { "busy_wait", Kernel::BUSY_WAIT },
                                                           { "compute", Kernel::COMPUTE },
                                                           { "firestarter", Kernel::FIRESTARTER },
                                                           { "blas", Kernel::BLAS },
                                                           { "memory_write", Kernel::MEMORY_WRITE },
                                                           { "memory_copy", Kernel::MEMORY_COPY },
                                                           { "memory_read", Kernel::MEMORY_READ },
                                                           { "sqrt", Kernel::SQRT } };

bowl::Expected<Kernel, bowl::CustomError> str_to_kernel(std::string kernel)
{
    assert(str_to_kernel_map.size() == (int)Kernel::_KERNEL_MAX);

    if (str_to_kernel_map.count(kernel) == 0)
    {
        return bowl::Unexpected(bowl::CustomError("Unknown kernel: " + kernel));
    }

    return static_cast<Kernel>(str_to_kernel_map.at(kernel));
}

std::vector<std::string> kernel_names()
{
    std::vector<std::string> res;
    for (auto kernel : str_to_kernel_map)
    {
        res.emplace_back(kernel.first);
    }
    return res;
}
} // namespace kernels

} // namespace roco2
