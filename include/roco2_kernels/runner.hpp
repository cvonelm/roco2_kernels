// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/base_kernel.hpp>
#include <roco2_kernels/gpu.hpp>

#include <penguinxx/clock.hpp>
#include <penguinxx/cpu.hpp>
#include <penguinxx/numa.hpp>
#include <penguinxx/pthread/barrier.hpp>
#include <penguinxx/pthread/thread.hpp>

#include <cassert>
#include <chrono>
#include <map>
#include <optional>

namespace roco2
{

namespace kernels
{
class Runner
{
public:
    Runner()
    {
    }

    struct result
    {
        uint64_t begin;
        uint64_t end;
        std::map<penguinxx::Cpu, uint64_t> it_count;
    };

    struct result run(std::chrono::seconds run_time)
    {
        int num_threads;
        if (gpu_.has_value())
        {
            num_threads = load_map_.size() + 1;
        }
        else
        {

            num_threads = load_map_.size();
        }
        penguinxx::Barrier barrier = penguinxx::Barrier::create(num_threads).unpack_ok();

        struct result res;

        std::vector<penguinxx::Pthread> threads;
        auto cpu_it = load_map_.begin();
        for (; cpu_it != --load_map_.end(); cpu_it++)
        {
            res.it_count.emplace(cpu_it->first, 0);

            threads.emplace_back(penguinxx::Pthread::create(run_kernel, cpu_it->first, false,
                                                            cpu_it->second, &barrier, run_time,
                                                            &res)
                                     .unpack_ok());
        }

        if (gpu_.has_value())
        {
            threads.emplace_back(
                penguinxx::Pthread::create(run_gpu, &gpu_.value(), run_time, &barrier).unpack_ok());
        }
        res.it_count.emplace(cpu_it->first, 0);

        // Run one of the kernel execution threads on the main thread
        run_kernel(cpu_it->first, true, cpu_it->second, &barrier, run_time, &res);

        for (auto& thread : threads)
        {
            thread.join();
        }

        return res;
    }

    void add_cpu(penguinxx::Cpu cpu, roco2::kernels::Kernel kernel)
    {
        assert(load_map_.emplace(cpu, kernel).second);
    }

    void add_gpu(int percentage)
    {
        gpu_ = gpu_kernel(percentage);
    }

private:
    static void run_kernel(penguinxx::Cpu cpu, bool is_main, roco2::kernels::Kernel kernel,
                           penguinxx::Barrier* barrier, std::chrono::seconds run_time,
                           struct result* res)
    {

        // Bind this thread to the given CPU
        cpu.bind_this_thread_to();

        // Use only allocations from the NUMA node of this CPU
        penguinxx::NUMANodeSet::of_cpu(cpu).unpack_ok().membind();

        auto k = roco2::kernels::kernel_from_enum(kernel);

        // Wait for the other threads to arrive
        barrier->wait();

        if (is_main)
        {
            res->begin = penguinxx::Clock::gettime(penguinxx::Clocks::MONOTONIC_RAW).unpack_ok();
        }

        k->run(std::chrono::high_resolution_clock::now() + run_time);

        if (is_main)
        {
            res->end = penguinxx::Clock::gettime(penguinxx::Clocks::MONOTONIC_RAW).unpack_ok();
        }

        res->it_count.at(cpu) = k->iteration_count();
    }

    static void run_gpu(gpu_kernel* gpu, std::chrono::seconds run_time, penguinxx::Barrier* barrier)
    {
        barrier->wait();
        gpu->run(std::chrono::high_resolution_clock::now() + run_time);
    }

    std::map<penguinxx::Cpu, roco2::kernels::Kernel> load_map_;
    std::optional<gpu_kernel> gpu_;
};
} // namespace kernels
} // namespace roco2
