# roco2_kernels -- Kernels from the roco2 workload generator

This repository contains extracted and modernized kernels from the
original [roco2](https://github.com/tud-zih-energy/roco2) for easy
integration into diverse experimentation workflows.

The workloads kernels defined by roco2 provide a diverse set of
operating points for processors that can aid in helping to verify
processor behavior (such as reported power consumption) over a wide
range of different operation points.

## Building

### Requirements

- A x86-64 or AArch64 processor for the Assembly kernels.
- A cblas implementation. Both OpenBLAS and netlib have been tested.
- Penguinxx requirements:
    - C++ fmtlib (https://github.com/fmtlib/fmt)
- FIRESTARTER requirements:
    - hwloc (https://www.open-mpi.org/projects/hwloc/)

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

## Running

USAGE: ./kernel_runner [kernel_name] [CPUS] [SECONDS]

- `kernel_name`: Name of the kernel. See the list of defined kernels below
- `CPUS`: Set of cpus on which the kernel will be executed. One instance
  of every kernel is executed on every specified processor core.
  This accepts range strings in the form of "3-5" or "1,2,3"
- `SECONDS`: For how long the kernel should be run.

### Kernels

- addpd -- SIMD ASM addition
- mulpd -- SIMD ASM multiplication
- sinus -- `sin()`
- sqrt -- square-root ASM calculation
- blas -- Matrix multiplication using a BLAS library
- busy_wait -- A busy_wait `while()` loop
- compute -- Manually implemented Matrix multiplication
- firestarter -- [FIRESTARTER](https://github.com/tud-zih-energy/FIRESTARTER) 
- idle -- `sleep()`
- memory_copy -- memory copy
- memory_read -- memory read
- memory_write -- memory write

## Pre-defined Experiments

`experiments/` contains a set of Experiments that can be run using
[exomat](https://github.com/tud-zih-energy/exomat)

## Acknowledgments and license

This software is based on the original [roco2](https://github.com/tud-zih-energy/roco2), developed by Mario Bielert, Hannes Tröpgen and Thomas Ilsche.

This work is based on "[Hackenberg et. al.: Power measurement techniques on standard compute nodes: A quantitative comparison](http://ieeexplore.ieee.org/abstract/document/6557170/)".

This work is licensed under the terms of the GPL 3.0, for more information see LICENSE.
