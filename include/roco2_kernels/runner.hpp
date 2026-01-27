// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/base_kernel.hpp>

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

    struct result_arg
    {
        uint64_t begin;
        uint64_t end;
        uint64_t it_count;
    };

    struct thread_arg
    {
        roco2::kernels::Kernel kernel;
        penguinxx::Cpu cpu;
        bool main_thread;
        penguinxx::Barrier& barrier;
        std::chrono::seconds run_time;
        struct result_arg res;
    };

    struct result_arg run(std::chrono::seconds run_time)
    {
        penguinxx::Barrier barrier = penguinxx::Barrier::create(load_map_.size()).unpack_ok();

        std::map<penguinxx::Cpu, struct thread_arg> args;

        struct result_arg res;
        for (auto& load : load_map_)
        {
            bool is_main = false;
            if (load.first == (--load_map_.end())->first)
            {
                is_main = true;
            }
            struct thread_arg arg{ .kernel = load.second,
                                   .cpu = load.first,
                                   .main_thread = is_main,
                                   .barrier = barrier,
                                   .run_time = run_time,
                                   .res = res };
            args.emplace(load.first, arg);
        }
        std::vector<penguinxx::Pthread> threads;
        auto cpu_it = args.begin();
        for (; cpu_it != --args.end(); cpu_it++)
        {
            threads.emplace_back(
                penguinxx::Pthread::create(run_kernel, (void*)&cpu_it->second).unpack_ok());
        }

        // Run one of the kernel execution threads on the main thread
        run_kernel((void*)&cpu_it->second);

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

private:
    static void* run_kernel(void* arg)
    {
        struct thread_arg* targ = reinterpret_cast<struct thread_arg*>(arg);

        // Bind this thread to the given CPU
        targ->cpu.bind_this_thread_to();

        // Use only allocations from the NUMA node of this CPU
        penguinxx::NUMANodeSet::of_cpu(targ->cpu).unpack_ok().membind();

        auto k = roco2::kernels::kernel_from_enum(targ->kernel);

        // Wait for the other threads to arrive
        targ->barrier.wait();

        if (targ->main_thread)
        {
            targ->res.begin =
                penguinxx::Clock::gettime(penguinxx::Clocks::MONOTONIC_RAW).unpack_ok();
        }
        k->run(std::chrono::high_resolution_clock::now() + targ->run_time);

        if (targ->main_thread)
        {
            targ->res.end = penguinxx::Clock::gettime(penguinxx::Clocks::MONOTONIC_RAW).unpack_ok();
            targ->res.it_count = k->iteration_count();
        }

        return nullptr;
    }

    std::optional<penguinxx::Barrier> barrier_;
    std::map<penguinxx::Cpu, roco2::kernels::Kernel> load_map_;
};
} // namespace kernels
} // namespace roco2
