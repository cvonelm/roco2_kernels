// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <roco2_kernels/base_kernel.hpp>

#ifdef __x86_64__
#include <firestarter/Environment/X86/X86Environment.hpp>
#elif __aarch64__
#include <firestarter/Environment/AArch64/AArch64Environment.hpp>
#else
#error "roco2 not implemented for this architecture!"
#endif

#include <chrono>
#include <memory>
#include <thread>

extern "C"
{
#include <sched.h>
}

namespace roco2
{
namespace kernels
{

class firestarter : public base_kernel
{
public:
    firestarter()
    {
        unsigned int cpu;
        getcpu(&cpu, nullptr);

        env.evaluateCpuAffinity(0, "");
        env.evaluateFunctions();
        env.selectFunction(0, false);
        lwd_ = std::make_unique<::firestarter::LoadWorkerData>(cpu, env, &loadVar, 0, false, false);

        auto* lwd = lwd_.get();

        auto dataCacheSizeIt = lwd->config().platformConfig().dataCacheBufferSize().begin();
        auto ramBufferSize = lwd->config().platformConfig().ramBufferSize();

        lwd->buffersizeMem = (*dataCacheSizeIt + *std::next(dataCacheSizeIt, 1) +
                              *std::next(dataCacheSizeIt, 2) + ramBufferSize) /
                             lwd->config().thread() / sizeof(unsigned long long);

        // Will be free'd in FIRESTARTER
        firestarter_buffer_ = (unsigned long long int*)aligned_alloc(
            64, lwd->buffersizeMem * sizeof(unsigned long long int));

        lwd->addrMem = firestarter_buffer_ + lwd->addrOffset;

        lwd->environment().setCpuAffinity(lwd->id());
        lwd->config().payload().compilePayload(
            lwd->config().payloadSettings(), lwd->config().instructionCacheSize(),
            lwd->config().dataCacheBufferSize(), lwd->config().ramBufferSize(),
            lwd->config().thread(), lwd->config().lines(), lwd->dumpRegisters, lwd->errorDetection);
        lwd->config().payload().init(lwd->addrMem, lwd->buffersizeMem);
    }

    static void stop(std::chrono::high_resolution_clock::time_point until,
                     unsigned long long* loadVar)

    {
        std::this_thread::sleep_for(until - std::chrono::high_resolution_clock::now());
        *loadVar = LOAD_STOP;
    }

    void run(std::chrono::high_resolution_clock::time_point until)
    {
        std::thread cntrl_thread;
        loadVar = LOAD_HIGH;
        cntrl_thread = std::thread(stop, until, &loadVar);

        auto* lwd = lwd_.get();
        uint64_t loops;
        do
        {
            lwd->iterations = lwd->config().payload().highLoadFunction(lwd->addrMem, lwd->addrHigh,
                                                                       lwd->iterations);

            loops++;

        } while (std::chrono::high_resolution_clock::now() < until);

        cntrl_thread.join();

        iteration_count_ = loops;
    }

    unsigned long long loadVar;

private:
    unsigned long long int* firestarter_buffer_;
    std::unique_ptr<::firestarter::LoadWorkerData> lwd_;

#ifdef __x86_64__
    ::firestarter::environment::x86::X86Environment env;
#elif __aarch64__
    ::firestarter::environment::aarch64::AArch64Environment env;
#endif
};

} // namespace kernels
} // namespace roco2
