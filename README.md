# roco2_kernels -- Kernels from the roco2 workload generator

This repository contains extracted and modernized kernels from the
original [roco2](https://github.com/tud-zih-energy/roco2) for easy
integration into diverse experimentation workflows.

The workloads kernels defined by roco2 provide a diverse set of
operating points for processors that can aid in helping to verify
processor behavior (such as reported power consumption) over a wide
range of different operation points.

## Running

`./kernel_runner [KERNEL] [CPUS] [SECONDS] [gpu [GPU_PERCENTAGE]`

- `KERNEL`: Name of the CPU kernel. See the list of defined kernels below
- `CPUS`: Set of cpus on which the CPU kernel will be executed.
  This accepts range strings in the form of "3-5" or "1,2,3"
- `SECONDS`: For how long roco2 should be run.
- `gpu [GPU_PERCENTAGE]` *OPTIONAL* If this argument is given, additionally
  to the CPU kernel, a GPU kernel is run on `GPU_PERCENTAGE` of all SM's.

### Kernels

- addpd -- assembly SIMD addition
- mulpd -- assembly SIMD multiplication
- sqrt -- assembly square-root calculation
- sinus -- continuous `sin()`
- blas -- Matrix multiplication using a BLAS library
- busy_wait -- A busy_wait `while()` loop
- compute -- Manually implemented Matrix multiplication
- firestarter -- [FIRESTARTER](https://github.com/tud-zih-energy/FIRESTARTER) 
- idle -- `sleep()`
- memory_copy -- copying memory between locations
- memory_read -- reading memory into registers
- memory_write -- writing registers into memory
  
### Examples

```bash
# Runs the addpd kernel for 10 seconds on CPUs 0, 2 and 3
~ % ./kernel_runner addpd 0,2,3 10
# Runs the blas matmul kernel on CPUs 0 through 10, while running
# the GPU kernel on 30% of all SM's
~ % ./kernel_runner blas 0-10 10 gpu 30
```

### Output

To integrate with other measurement infrastructure, mostly [exomat](https://github.com/tud-zih-energy/exomat), roco2_kernels provides output of its kernel execution in the form of `out_*` files.

Those are:

- `out_ts_begin`: The beginning of the execution of the CPU kernel, in
  clock-ticks as recorded through the `MONOTONIC_RAW` clock.
- `out_ts_end`: The end of the execution of the CPU kernel, in
  clock-ticks as recorded through the `MONOTONIC_RAW` clock.
- `out_iteration_count` The number of executions of the CPU kernel, summed up
  over all CPUs. Every roco2 workload is implemented around a small
  kernel which is continuously executed until the specified run-time has
  passed. This iteration count can then be used to gauge system performance
  under these workloads.

### Additional Commands

These have been developed for one-off experiments:

```bash
# Execute the compute kernel continuously, while reducing the
# CPU frequency from the highest to the lowest in 2 second intervals.
~ % ./frequency_sweeper
```
## Building

### Requirements

- A x86-64 or AArch64 processor for the Assembly kernels.
- A cblas implementation. Both OpenBLAS and netlib have been tested.
- Penguinxx requirements:
    - C++ fmtlib (https://github.com/fmtlib/fmt)
- FIRESTARTER requirements:
    - hwloc (https://www.open-mpi.org/projects/hwloc/)
- _OPTIONAL_: CUDA, for the optional GPU workload

roco2_kernels is dependent on submodules.
To be able to build it, be sure you have downloaded them

```
$ git submodule update --init --recursive
```

Building is done with cmake:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Pre-defined Experiments

`experiments/` contains a set of Experiments that can be run using
[exomat](https://github.com/tud-zih-energy/exomat)

## Acknowledgments and license

This software is based on the original [roco2](https://github.com/tud-zih-energy/roco2), developed by Mario Bielert, Hannes Tröpgen and Thomas Ilsche.

This work is based on "[Hackenberg et. al.: Power measurement techniques on standard compute nodes: A quantitative comparison](http://ieeexplore.ieee.org/abstract/document/6557170/)".

This work is licensed under the terms of the GPL 3.0, for more information see LICENSE.
