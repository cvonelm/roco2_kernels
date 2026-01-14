// SPDX-License-Identifier: GPL-3.0

#include <roco2_kernels/asm_kernels.hpp>

#include <cassert>
#include <cstdint>

uint64_t mulpd_kernel(double* buffer, uint64_t repeat)
{
    unsigned long long passes, addr;
    unsigned long long a, b, c, d;
    uint64_t ret = 0;

    passes = repeat / 32; // 32 128-Bit accesses in inner loop

    addr = (unsigned long long)buffer;

    if (!passes)
        return ret;
    /*
     * Input:  RAX: addr (pointer to the buffer)
     *         RBX: passes (number of iterations)
     *         RCX: length (total number of accesses)
     */
    __asm__ __volatile__(

        // initialize registers
        "ld4 {v0.2d-v3.2d}, [%0], #64\n\t"
        "ld4 {v4.2d-v7.2d}, [%0], #64\n\t"
        "ld4 {v8.2d-v11.2d}, [%0], #64\n\t"
        "ld4 {v12.2d-v15.2d}, [%0], #64\n\t"

        "_work_loop_mul_pd:"
        "fmul v8.2d, v0.2d, v8.2d\n\t"
        "fmul v9.2d, v1.2d, v9.2d\n\t"
        "fmul v10.2d, v2.2d, v10.2d\n\t"
        "fmul v11.2d, v3.2d, v11.2d\n\t"
        "fmul v12.2d, v4.2d, v12.2d\n\t"
        "fmul v13.2d, v5.2d, v13.2d\n\t"
        "fmul v14.2d, v6.2d, v14.2d\n\t"
        "fmul v15.2d, v7.2d, v15.2d\n\t"

        "fmul v8.2d, v0.2d, v8.2d\n\t"
        "fmul v9.2d, v1.2d, v9.2d\n\t"
        "fmul v10.2d, v2.2d, v10.2d\n\t"
        "fmul v11.2d, v3.2d, v11.2d\n\t"
        "fmul v12.2d, v4.2d, v12.2d\n\t"
        "fmul v13.2d, v5.2d, v13.2d\n\t"
        "fmul v14.2d, v6.2d, v14.2d\n\t"
        "fmul v15.2d, v7.2d, v15.2d\n\t"

        "fmul v8.2d, v0.2d, v8.2d\n\t"
        "fmul v9.2d, v1.2d, v9.2d\n\t"
        "fmul v10.2d, v2.2d, v10.2d\n\t"
        "fmul v11.2d, v3.2d, v11.2d\n\t"
        "fmul v12.2d, v4.2d, v12.2d\n\t"
        "fmul v13.2d, v5.2d, v13.2d\n\t"
        "fmul v14.2d, v6.2d, v14.2d\n\t"
        "fmul v15.2d, v7.2d, v15.2d\n\t"

        "fmul v8.2d, v0.2d, v8.2d\n\t"
        "fmul v9.2d, v1.2d, v9.2d\n\t"
        "fmul v10.2d, v2.2d, v10.2d\n\t"
        "fmul v11.2d, v3.2d, v11.2d\n\t"
        "fmul v12.2d, v4.2d, v12.2d\n\t"
        "fmul v13.2d, v5.2d, v13.2d\n\t"
        "fmul v14.2d, v6.2d, v14.2d\n\t"
        "fmul v15.2d, v7.2d, v15.2d\n\t"

        "sub %1,%1,#1\n\t"
        "cmp %1, #0\n\t"
        "bne _work_loop_mul_pd\n\t"

        :
        : "r"(addr), "r"(passes)
        : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13",
          "v14", "v15"

    );
    ret = passes;

    return ret;
}

uint64_t addpd_kernel(double* buffer, uint64_t repeat)
{
    unsigned long long passes, addr;
    unsigned long long a, b, c, d;
    uint64_t ret = 0;

    passes = repeat / 32; // 32 128-Bit accesses in inner loop

    addr = (unsigned long long)buffer;

    if (!passes)
        return ret;
    /*
     * Input:  RAX: addr (pointer to the buffer)
     *         RBX: passes (number of iterations)
     *         RCX: length (total number of accesses)
     */
    __asm__ __volatile__(

        // initialize registers
        "ld4 {v0.2d-v3.2d}, [%0], #64\n\t"
        "ld4 {v4.2d-v7.2d}, [%0], #64\n\t"
        "ld4 {v8.2d-v11.2d}, [%0], #64\n\t"
        "ld4 {v12.2d-v15.2d}, [%0], #64\n\t"

        "_work_loop_add_pd:"
        "fadd v8.2d, v0.2d, v8.2d\n\t"
        "fadd v9.2d, v1.2d, v9.2d\n\t"
        "fadd v10.2d, v2.2d, v10.2d\n\t"
        "fadd v11.2d, v3.2d, v11.2d\n\t"
        "fadd v12.2d, v4.2d, v12.2d\n\t"
        "fadd v13.2d, v5.2d, v13.2d\n\t"
        "fadd v14.2d, v6.2d, v14.2d\n\t"
        "fadd v15.2d, v7.2d, v15.2d\n\t"

        "fadd v8.2d, v0.2d, v8.2d\n\t"
        "fadd v9.2d, v1.2d, v9.2d\n\t"
        "fadd v10.2d, v2.2d, v10.2d\n\t"
        "fadd v11.2d, v3.2d, v11.2d\n\t"
        "fadd v12.2d, v4.2d, v12.2d\n\t"
        "fadd v13.2d, v5.2d, v13.2d\n\t"
        "fadd v14.2d, v6.2d, v14.2d\n\t"
        "fadd v15.2d, v7.2d, v15.2d\n\t"

        "fadd v8.2d, v0.2d, v8.2d\n\t"
        "fadd v9.2d, v1.2d, v9.2d\n\t"
        "fadd v10.2d, v2.2d, v10.2d\n\t"
        "fadd v11.2d, v3.2d, v11.2d\n\t"
        "fadd v12.2d, v4.2d, v12.2d\n\t"
        "fadd v13.2d, v5.2d, v13.2d\n\t"
        "fadd v14.2d, v6.2d, v14.2d\n\t"
        "fadd v15.2d, v7.2d, v15.2d\n\t"

        "fadd v8.2d, v0.2d, v8.2d\n\t"
        "fadd v9.2d, v1.2d, v9.2d\n\t"
        "fadd v10.2d, v2.2d, v10.2d\n\t"
        "fadd v11.2d, v3.2d, v11.2d\n\t"
        "fadd v12.2d, v4.2d, v12.2d\n\t"
        "fadd v13.2d, v5.2d, v13.2d\n\t"
        "fadd v14.2d, v6.2d, v14.2d\n\t"
        "fadd v15.2d, v7.2d, v15.2d\n\t"

        "sub %1,%1,#1\n\t"
        "cmp %1, #0\n\t"
        "bne _work_loop_add_pd\n\t"

        :
        : "r"(addr), "r"(passes)
        : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13",
          "v14", "v15"

    );
    ret = passes;

    return ret;
}

uint64_t sqrtsd_kernel(double* buffer, uint64_t elems, uint64_t repeat)
{
    unsigned long long passes, length, addr;
    unsigned long long a, b, c, d;
    uint64_t ret = 0;
    assert(elems >= 256 / sizeof(*buffer));

    passes = elems / 64; // 32 128-Bit accesses in inner loop
    length = passes * 32 * repeat;
    addr = (unsigned long long)buffer;

    if (!passes)
        return ret;
    /*
     * Input:  RAX: addr (pointer to the buffer)
     *         RBX: passes (number of iterations)
     *         RCX: length (total number of accesses)
     */
    __asm__ __volatile__(
        // initialize registers
        "ld4 {v0.2d-v3.2d}, [%[addr]], #64\n\t"
        "ld4 {v4.2d-v7.2d}, [%[addr]], #64\n\t"
        "ld4 {v8.2d-v11.2d}, [%[addr]], #64\n\t"
        "ld4 {v12.2d-v15.2d}, [%[addr]], #64\n\t"

        "_work_loop_sqrt_sd:"
        "fsqrt v8.2d, v0.2d\n\t"
        "fsqrt v9.2d, v0.2d\n\t"
        "fsqrt v10.2d, v0.2d\n\t"
        "fsqrt v11.2d, v0.2d\n\t"
        "fsqrt v12.2d, v0.2d\n\t"
        "fsqrt v13.2d, v0.2d\n\t"
        "fsqrt v14.2d, v0.2d\n\t"
        "fsqrt v15.2d, v0.2d\n\t"
        "fsqrt v8.2d, v0.2d\n\t"
        "fsqrt v9.2d, v0.2d\n\t"
        "fsqrt v10.2d, v0.2d\n\t"
        "fsqrt v11.2d, v0.2d\n\t"
        "fsqrt v12.2d, v0.2d\n\t"
        "fsqrt v13.2d, v0.2d\n\t"
        "fsqrt v14.2d, v0.2d\n\t"
        "fsqrt v15.2d, v0.2d\n\t"
        "fsqrt v8.2d, v0.2d\n\t"
        "fsqrt v9.2d, v0.2d\n\t"
        "fsqrt v10.2d, v0.2d\n\t"
        "fsqrt v11.2d, v0.2d\n\t"
        "fsqrt v12.2d, v0.2d\n\t"
        "fsqrt v13.2d, v0.2d\n\t"
        "fsqrt v14.2d, v0.2d\n\t"
        "fsqrt v15.2d, v0.2d\n\t"
        "fsqrt v8.2d, v0.2d\n\t"
        "fsqrt v9.2d, v0.2d\n\t"
        "fsqrt v10.2d, v0.2d\n\t"
        "fsqrt v11.2d, v0.2d\n\t"
        "fsqrt v12.2d, v0.2d\n\t"
        "fsqrt v13.2d, v0.2d\n\t"
        "fsqrt v14.2d, v0.2d\n\t"
        "fsqrt v15.2d, v0.2d\n\t"
        "sub %[length], %[length], #32;\n\r"
        "cmp %[length], #0\n\t"
        "bne _work_loop_sqrt_sd\n\t"
        : "=r"(a), "=r"(b), "=r"(c), "=r"(d)
        : [addr] "r"(addr), [passes] "r"(passes), [length] "r"(length)
        : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13",
          "v14", "v15"

    );
    ret = c;

    return ret;
}
