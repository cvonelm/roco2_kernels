// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/base_kernel.hpp>

#include <penguinxx/clock.hpp>
#include <penguinxx/cpu_set.hpp>
#include <penguinxx/numa.hpp>
#include <penguinxx/pthread/barrier.hpp>
#include <penguinxx/pthread/thread.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>

int run_time;
std::optional<penguinxx::Barrier> barrier_;

uint64_t begin, end, iteration_count;

std::optional<penguinxx::Cpu> main_thread_cpu;
struct thread_arg
{
    roco2::kernels::Kernel k;
    penguinxx::Cpu c;
};

void* run_kernel(void* arg)
{
    struct thread_arg* targ = reinterpret_cast<struct thread_arg*>(arg);

    // Bind this thread to the given CPU
    targ->c.bind_this_thread_to();

    // Use only allocations from the NUMA node of this CPU
    penguinxx::NUMANodeSet::of_cpu(targ->c).unpack_ok().membind();

    auto k = roco2::kernels::kernel_from_enum(targ->k);

    // Wait for the other threads to arrive
    barrier_->wait();

    if (main_thread_cpu.value() == targ->c)
    {
        begin = penguinxx::Clock::gettime(penguinxx::Clocks::MONOTONIC_RAW).unpack_ok();
    }
    k->run(std::chrono::high_resolution_clock::now() + std::chrono::seconds(run_time));

    if (main_thread_cpu.value() == targ->c)
    {
        end = penguinxx::Clock::gettime(penguinxx::Clocks::MONOTONIC_RAW).unpack_ok();
        iteration_count = k->iteration_count();
    }

    return nullptr;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "USAGE: ./kernel_runner [kernel_name] [CPUS] [SECONDS]\n";
        std::cerr << "With kernel_name:\n";

        for (auto kernel : roco2::kernels::kernel_names())
        {
            std::cerr << "\t- " << kernel << std::endl;
        }

        return 1;
    }

    auto kernel_res = roco2::kernels::str_to_kernel(argv[1]);
    if (!kernel_res.ok())
    {
        std::cerr << "Could not parse kernel name: " << argv[1] << std::endl;
        return 1;
    }
    auto kernel = kernel_res.unpack_ok();

    auto cpu_set = penguinxx::CpuSet::from_range_str(argv[2]).unpack_ok();

    run_time = std::stoul(argv[3]);

    barrier_ = penguinxx::Barrier::create(cpu_set.size()).unpack_ok();

    std::map<penguinxx::Cpu, struct thread_arg> args;
    for (const auto& cpu : cpu_set)
    {
        struct thread_arg targ = { .k = kernel, .c = cpu };
        args.emplace(cpu, targ);
    }

    std::vector<penguinxx::Pthread> threads;
    auto cpu_it = cpu_set.begin();
    for (; cpu_it != --cpu_set.end(); cpu_it++)
    {
        threads.emplace_back(
            penguinxx::Pthread::create(run_kernel, (void*)&args.at(*cpu_it)).unpack_ok());
    }

    // Run one of the kernel execution threads on the main thread
    main_thread_cpu = *cpu_it;
    run_kernel((void*)&args.at(*main_thread_cpu));

    for (auto& thread : threads)
    {
        thread.join();
    }

    // Write timestamp begin/end markers that
    //  can be later used to correlate events from other
    //  sources
    std::ofstream begin_file("out_ts_begin");
    std::ofstream end_file("out_ts_end");
    std::ofstream iteration_count_file("out_iteration_count");

    begin_file << begin;
    end_file << end;
    iteration_count_file << iteration_count;

    return 0;
}
