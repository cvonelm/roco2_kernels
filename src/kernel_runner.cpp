// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/base_kernel.hpp>
#include <roco2_kernels/runner.hpp>

#include <penguinxx/cpu.hpp>
#include <penguinxx/cpu_set.hpp>

#include <chrono>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 4 && argc != 6)
    {
        std::cerr << "USAGE: ./kernel_runner [kernel_name] [CPUS] [SECONDS] [gpu [PERCENTAGE]]\n";
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
    auto run_time = std::chrono::seconds(std::stoul(argv[3]));

    roco2::kernels::Runner r;
    for (const auto& cpu : cpu_set)
    {
        r.add_cpu(cpu, kernel);
    }

    if (argc == 6)
    {
        r.add_gpu(std::stoi(argv[5]));
    }

    auto res = r.run(run_time);

    // Write timestamp begin/end markers that
    //  can be later used to correlate events from other
    //  sources
    std::ofstream begin_file("out_ts_begin");
    std::ofstream end_file("out_ts_end");
    std::ofstream iteration_count_file("out_iteration_count");

    begin_file << res.begin;
    end_file << res.end;
    iteration_count_file << res.it_count;

    return 0;
}
