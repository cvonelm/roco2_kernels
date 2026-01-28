// SPDX-License-Identifier: GPL-3.0

// ./frequency_sweeper.
//
// Runs the 'compute' kernel while gradually reducing frequency from
// the highest to the lowest setting.
#include <roco2_kernels/runner.hpp>

#include <penguinxx/cpu.hpp>
#include <penguinxx/topology.hpp>

#include <iostream>
#include <thread>

void freq_change()
{
    auto frequencies = penguinxx::Cpu::current().unpack_ok().available_frequencies().unpack_ok();

    std::cout << "NUM FREQUENCIES: " << frequencies.size();
    for (auto freq : frequencies)
    {
        for (auto cpu : penguinxx::CpuTopology::instance().cpus())
        {
            cpu.frequency(freq);
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main(void)
{
    roco2::kernels::Runner r;

    for (auto& cpu : penguinxx::CpuTopology::instance().cpus())
    {
        r.add_cpu(cpu, roco2::kernels::Kernel::COMPUTE);
    }

    std::thread freq_change_t(freq_change);

    // TODO needs to be adjusted to be longer than the time
    // for all frequency changes. Find a way to automate
    r.run(std::chrono::seconds(10));
    freq_change_t.join();
}
