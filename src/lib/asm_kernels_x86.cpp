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
    __asm__ __volatile__("mov %%rax,%%r9;"  // addr
                         "mov %%rbx,%%r10;" // passes

                         // initialize registers
                         "movaps 0(%%r9), %%xmm0;"
                         "movaps 16(%%r9), %%xmm1;"
                         "movaps 32(%%r9), %%xmm2;"
                         "movaps 48(%%r9), %%xmm3;"
                         "movaps 64(%%r9), %%xmm4;"
                         "movaps 80(%%r9), %%xmm5;"
                         "movaps 96(%%r9), %%xmm6;"
                         "movaps 112(%%r9), %%xmm7;"
                         "movapd 0(%%r9), %%xmm8;"
                         "movapd 16(%%r9), %%xmm9;"
                         "movapd 32(%%r9), %%xmm10;"
                         "movapd 48(%%r9), %%xmm11;"
                         "movapd 64(%%r9), %%xmm12;"
                         "movapd 80(%%r9), %%xmm13;"
                         "movapd 96(%%r9), %%xmm14;"
                         "movapd 112(%%r9), %%xmm15;"

                         ".align 64;"
                         "_work_loop_mul_pd:"
                         "mulpd %%xmm8, %%xmm0;"
                         "mulpd %%xmm9, %%xmm1;"
                         "mulpd %%xmm10, %%xmm2;"
                         "mulpd %%xmm11, %%xmm3;"
                         "mulpd %%xmm12, %%xmm4;"
                         "mulpd %%xmm13, %%xmm5;"
                         "mulpd %%xmm14, %%xmm6;"
                         "mulpd %%xmm15, %%xmm7;"

                         "mulpd %%xmm8, %%xmm0;"
                         "mulpd %%xmm9, %%xmm1;"
                         "mulpd %%xmm10, %%xmm2;"
                         "mulpd %%xmm11, %%xmm3;"
                         "mulpd %%xmm12, %%xmm4;"
                         "mulpd %%xmm13, %%xmm5;"
                         "mulpd %%xmm14, %%xmm6;"
                         "mulpd %%xmm15, %%xmm7;"

                         "mulpd %%xmm8, %%xmm0;"
                         "mulpd %%xmm9, %%xmm1;"
                         "mulpd %%xmm10, %%xmm2;"
                         "mulpd %%xmm11, %%xmm3;"
                         "mulpd %%xmm12, %%xmm4;"
                         "mulpd %%xmm13, %%xmm5;"
                         "mulpd %%xmm14, %%xmm6;"
                         "mulpd %%xmm15, %%xmm7;"

                         "mulpd %%xmm8, %%xmm0;"
                         "mulpd %%xmm9, %%xmm1;"
                         "mulpd %%xmm10, %%xmm2;"
                         "mulpd %%xmm11, %%xmm3;"
                         "mulpd %%xmm12, %%xmm4;"
                         "mulpd %%xmm13, %%xmm5;"
                         "mulpd %%xmm14, %%xmm6;"
                         "mulpd %%xmm15, %%xmm7;"

                         "sub $1,%%r10;"
                         "jnz _work_loop_mul_pd;"

                         : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                         : "a"(addr), "b"(passes)
                         : "%r9", "%r10", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6",
                           "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14",
                           "xmm15"

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
    __asm__ __volatile__("mov %%rax,%%r9;"  // addr
                         "mov %%rbx,%%r10;" // passes

                         // initialize registers
                         "movaps 0(%%r9), %%xmm0;"
                         "movaps 16(%%r9), %%xmm1;"
                         "movaps 32(%%r9), %%xmm2;"
                         "movaps 48(%%r9), %%xmm3;"
                         "movaps 64(%%r9), %%xmm4;"
                         "movaps 80(%%r9), %%xmm5;"
                         "movaps 96(%%r9), %%xmm6;"
                         "movaps 112(%%r9), %%xmm7;"
                         "movapd 0(%%r9), %%xmm8;"
                         "movapd 16(%%r9), %%xmm9;"
                         "movapd 32(%%r9), %%xmm10;"
                         "movapd 48(%%r9), %%xmm11;"
                         "movapd 64(%%r9), %%xmm12;"
                         "movapd 80(%%r9), %%xmm13;"
                         "movapd 96(%%r9), %%xmm14;"
                         "movapd 112(%%r9), %%xmm15;"

                         ".align 64;"
                         "_work_loop_add_pd:"
                         "addpd %%xmm8, %%xmm0;"
                         "addpd %%xmm9, %%xmm1;"
                         "addpd %%xmm10, %%xmm2;"
                         "addpd %%xmm11, %%xmm3;"
                         "addpd %%xmm12, %%xmm4;"
                         "addpd %%xmm13, %%xmm5;"
                         "addpd %%xmm14, %%xmm6;"
                         "addpd %%xmm15, %%xmm7;"

                         "addpd %%xmm8, %%xmm0;"
                         "addpd %%xmm9, %%xmm1;"
                         "addpd %%xmm10, %%xmm2;"
                         "addpd %%xmm11, %%xmm3;"
                         "addpd %%xmm12, %%xmm4;"
                         "addpd %%xmm13, %%xmm5;"
                         "addpd %%xmm14, %%xmm6;"
                         "addpd %%xmm15, %%xmm7;"

                         "addpd %%xmm8, %%xmm0;"
                         "addpd %%xmm9, %%xmm1;"
                         "addpd %%xmm10, %%xmm2;"
                         "addpd %%xmm11, %%xmm3;"
                         "addpd %%xmm12, %%xmm4;"
                         "addpd %%xmm13, %%xmm5;"
                         "addpd %%xmm14, %%xmm6;"
                         "addpd %%xmm15, %%xmm7;"

                         "addpd %%xmm8, %%xmm0;"
                         "addpd %%xmm9, %%xmm1;"
                         "addpd %%xmm10, %%xmm2;"
                         "addpd %%xmm11, %%xmm3;"
                         "addpd %%xmm12, %%xmm4;"
                         "addpd %%xmm13, %%xmm5;"
                         "addpd %%xmm14, %%xmm6;"
                         "addpd %%xmm15, %%xmm7;"

                         "sub $1,%%r10;"
                         "jnz _work_loop_add_pd;"

                         : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                         : "a"(addr), "b"(passes)
                         : "%r9", "%r10", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6",
                           "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14",
                           "xmm15"

    );
    ret = passes;

    return ret;
}

uint64_t sqrtss_kernel(float* buffer, uint64_t elems, uint64_t repeat)
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
    __asm__ __volatile__("mfence;"
                         "mov %%rax,%%r9;"  // addr
                         "mov %%rbx,%%r10;" // passes
                         "mov %%rcx,%%r15;" // length
                         "mov %%r9,%%r14;"  // store addr
                         "mov %%r10,%%r8;"  // store passes
                         "mov %%r15,%%r13;" // store length

                         // initialize registers
                         "movaps 0(%%r9), %%xmm0;"
                         "movaps 0(%%r9), %%xmm8;"
                         "movaps 16(%%r9), %%xmm9;"
                         "movaps 32(%%r9), %%xmm10;"
                         "movaps 48(%%r9), %%xmm11;"
                         "movaps 64(%%r9), %%xmm12;"
                         "movaps 80(%%r9), %%xmm13;"
                         "movaps 96(%%r9), %%xmm14;"
                         "movaps 112(%%r9), %%xmm15;"

                         ".align 64;"
                         "_work_loop_sqrt_ss:"
                         "sqrtss %%xmm8, %%xmm0;"
                         "sqrtss %%xmm9, %%xmm0;"
                         "sqrtss %%xmm10, %%xmm0;"
                         "sqrtss %%xmm11, %%xmm0;"
                         "sqrtss %%xmm12, %%xmm0;"
                         "sqrtss %%xmm13, %%xmm0;"
                         "sqrtss %%xmm14, %%xmm0;"
                         "sqrtss %%xmm15, %%xmm0;"
                         "sqrtss %%xmm8, %%xmm0;"
                         "sqrtss %%xmm9, %%xmm0;"
                         "sqrtss %%xmm10, %%xmm0;"
                         "sqrtss %%xmm11, %%xmm0;"
                         "sqrtss %%xmm12, %%xmm0;"
                         "sqrtss %%xmm13, %%xmm0;"
                         "sqrtss %%xmm14, %%xmm0;"
                         "sqrtss %%xmm15, %%xmm0;"
                         "sqrtss %%xmm8, %%xmm0;"
                         "sqrtss %%xmm9, %%xmm0;"
                         "sqrtss %%xmm10, %%xmm0;"
                         "sqrtss %%xmm11, %%xmm0;"
                         "sqrtss %%xmm12, %%xmm0;"
                         "sqrtss %%xmm13, %%xmm0;"
                         "sqrtss %%xmm14, %%xmm0;"
                         "sqrtss %%xmm15, %%xmm0;"
                         "sqrtss %%xmm8, %%xmm0;"
                         "sqrtss %%xmm9, %%xmm0;"
                         "sqrtss %%xmm10, %%xmm0;"
                         "sqrtss %%xmm11, %%xmm0;"
                         "sqrtss %%xmm12, %%xmm0;"
                         "sqrtss %%xmm13, %%xmm0;"
                         "sqrtss %%xmm14, %%xmm0;"
                         "sqrtss %%xmm15, %%xmm0;"
                         "add $512,%%r9;"
                         "sub $1,%%r10;"
                         "jnz _skip_reset_sqrt_ss;" // reset buffer if the end is reached
                         "mov %%r14,%%r9;"          // restore addr
                         "mov %%r8,%%r10;"          // restore passes
                         "_skip_reset_sqrt_ss:"
                         "sub $32,%%r15;"
                         "jnz _work_loop_sqrt_ss;"

                         "mov %%r13,%%rcx;" // restore length
                         : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                         : "a"(addr), "b"(passes), "c"(length)
                         : "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "xmm0",
                           "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9",
                           "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"

    );
    ret = c;

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
    __asm__ __volatile__("mfence;"
                         "mov %%rax,%%r9;"  // addr
                         "mov %%rbx,%%r10;" // passes
                         "mov %%rcx,%%r15;" // length
                         "mov %%r9,%%r14;"  // store addr
                         "mov %%r10,%%r8;"  // store passes
                         "mov %%r15,%%r13;" // store length

                         // initialize registers
                         "movapd 0(%%r9), %%xmm0;"
                         "movapd 0(%%r9), %%xmm8;"
                         "movapd 16(%%r9), %%xmm9;"
                         "movapd 32(%%r9), %%xmm10;"
                         "movapd 48(%%r9), %%xmm11;"
                         "movapd 64(%%r9), %%xmm12;"
                         "movapd 80(%%r9), %%xmm13;"
                         "movapd 96(%%r9), %%xmm14;"
                         "movapd 112(%%r9), %%xmm15;"

                         ".align 64;"
                         "_work_loop_sqrt_sd:"
                         "sqrtsd %%xmm8, %%xmm0;"
                         "sqrtsd %%xmm9, %%xmm0;"
                         "sqrtsd %%xmm10, %%xmm0;"
                         "sqrtsd %%xmm11, %%xmm0;"
                         "sqrtsd %%xmm12, %%xmm0;"
                         "sqrtsd %%xmm13, %%xmm0;"
                         "sqrtsd %%xmm14, %%xmm0;"
                         "sqrtsd %%xmm15, %%xmm0;"
                         "sqrtsd %%xmm8, %%xmm0;"
                         "sqrtsd %%xmm9, %%xmm0;"
                         "sqrtsd %%xmm10, %%xmm0;"
                         "sqrtsd %%xmm11, %%xmm0;"
                         "sqrtsd %%xmm12, %%xmm0;"
                         "sqrtsd %%xmm13, %%xmm0;"
                         "sqrtsd %%xmm14, %%xmm0;"
                         "sqrtsd %%xmm15, %%xmm0;"
                         "sqrtsd %%xmm8, %%xmm0;"
                         "sqrtsd %%xmm9, %%xmm0;"
                         "sqrtsd %%xmm10, %%xmm0;"
                         "sqrtsd %%xmm11, %%xmm0;"
                         "sqrtsd %%xmm12, %%xmm0;"
                         "sqrtsd %%xmm13, %%xmm0;"
                         "sqrtsd %%xmm14, %%xmm0;"
                         "sqrtsd %%xmm15, %%xmm0;"
                         "sqrtsd %%xmm8, %%xmm0;"
                         "sqrtsd %%xmm9, %%xmm0;"
                         "sqrtsd %%xmm10, %%xmm0;"
                         "sqrtsd %%xmm11, %%xmm0;"
                         "sqrtsd %%xmm12, %%xmm0;"
                         "sqrtsd %%xmm13, %%xmm0;"
                         "sqrtsd %%xmm14, %%xmm0;"
                         "sqrtsd %%xmm15, %%xmm0;"
                         "add $512,%%r9;"
                         "sub $1,%%r10;"
                         "jnz _skip_reset_sqrt_sd;" // reset buffer if the end is reached
                         "mov %%r14,%%r9;"          // restore addr
                         "mov %%r8,%%r10;"          // restore passes
                         "_skip_reset_sqrt_sd:"
                         "sub $32,%%r15;"
                         "jnz _work_loop_sqrt_sd;"

                         "mov %%r13,%%rcx;" // restore length
                         : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                         : "a"(addr), "b"(passes), "c"(length)
                         : "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "xmm0",
                           "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9",
                           "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"

    );
    ret = c;

    return ret;
}

uint64_t sqrtps_kernel(float* buffer, uint64_t elems, uint64_t repeat)
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
    __asm__ __volatile__("mfence;"
                         "mov %%rax,%%r9;"  // addr
                         "mov %%rbx,%%r10;" // passes
                         "mov %%rcx,%%r15;" // length
                         "mov %%r9,%%r14;"  // store addr
                         "mov %%r10,%%r8;"  // store passes
                         "mov %%r15,%%r13;" // store length

                         // initialize registers
                         "movaps 0(%%r9), %%xmm0;"
                         "movaps 0(%%r9), %%xmm8;"
                         "movaps 16(%%r9), %%xmm9;"
                         "movaps 32(%%r9), %%xmm10;"
                         "movaps 48(%%r9), %%xmm11;"
                         "movaps 64(%%r9), %%xmm12;"
                         "movaps 80(%%r9), %%xmm13;"
                         "movaps 96(%%r9), %%xmm14;"
                         "movaps 112(%%r9), %%xmm15;"

                         ".align 64;"
                         "_work_loop_sqrt_ps:"
                         "sqrtps %%xmm8, %%xmm0;"
                         "sqrtps %%xmm9, %%xmm0;"
                         "sqrtps %%xmm10, %%xmm0;"
                         "sqrtps %%xmm11, %%xmm0;"
                         "sqrtps %%xmm12, %%xmm0;"
                         "sqrtps %%xmm13, %%xmm0;"
                         "sqrtps %%xmm14, %%xmm0;"
                         "sqrtps %%xmm15, %%xmm0;"
                         "sqrtps %%xmm8, %%xmm0;"
                         "sqrtps %%xmm9, %%xmm0;"
                         "sqrtps %%xmm10, %%xmm0;"
                         "sqrtps %%xmm11, %%xmm0;"
                         "sqrtps %%xmm12, %%xmm0;"
                         "sqrtps %%xmm13, %%xmm0;"
                         "sqrtps %%xmm14, %%xmm0;"
                         "sqrtps %%xmm15, %%xmm0;"
                         "sqrtps %%xmm8, %%xmm0;"
                         "sqrtps %%xmm9, %%xmm0;"
                         "sqrtps %%xmm10, %%xmm0;"
                         "sqrtps %%xmm11, %%xmm0;"
                         "sqrtps %%xmm12, %%xmm0;"
                         "sqrtps %%xmm13, %%xmm0;"
                         "sqrtps %%xmm14, %%xmm0;"
                         "sqrtps %%xmm15, %%xmm0;"
                         "sqrtps %%xmm8, %%xmm0;"
                         "sqrtps %%xmm9, %%xmm0;"
                         "sqrtps %%xmm10, %%xmm0;"
                         "sqrtps %%xmm11, %%xmm0;"
                         "sqrtps %%xmm12, %%xmm0;"
                         "sqrtps %%xmm13, %%xmm0;"
                         "sqrtps %%xmm14, %%xmm0;"
                         "sqrtps %%xmm15, %%xmm0;"
                         "add $512,%%r9;"
                         "sub $1,%%r10;"
                         "jnz _skip_reset_sqrt_ps;" // reset buffer if the end is reached
                         "mov %%r14,%%r9;"          // restore addr
                         "mov %%r8,%%r10;"          // restore passes
                         "_skip_reset_sqrt_ps:"
                         "sub $32,%%r15;"
                         "jnz _work_loop_sqrt_ps;"

                         "mov %%r13,%%rcx;" // restore length
                         : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                         : "a"(addr), "b"(passes), "c"(length)
                         : "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "xmm0",
                           "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9",
                           "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"

    );
    ret = 4 * c;

    return ret;
}

uint64_t sqrtpd_kernel(double* buffer, uint64_t elems, uint64_t repeat)
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
    __asm__ __volatile__("mfence;"
                         "mov %%rax,%%r9;"  // addr
                         "mov %%rbx,%%r10;" // passes
                         "mov %%rcx,%%r15;" // length
                         "mov %%r9,%%r14;"  // store addr
                         "mov %%r10,%%r8;"  // store passes
                         "mov %%r15,%%r13;" // store length

                         // initialize registers
                         "movaps 0(%%r9), %%xmm0;"
                         "movapd 0(%%r9), %%xmm8;"
                         "movapd 16(%%r9), %%xmm9;"
                         "movapd 32(%%r9), %%xmm10;"
                         "movapd 48(%%r9), %%xmm11;"
                         "movapd 64(%%r9), %%xmm12;"
                         "movapd 80(%%r9), %%xmm13;"
                         "movapd 96(%%r9), %%xmm14;"
                         "movapd 112(%%r9), %%xmm15;"

                         ".align 64;"
                         "_work_loop_sqrt_pd:"
                         "sqrtpd %%xmm8, %%xmm0;"
                         "sqrtpd %%xmm9, %%xmm0;"
                         "sqrtpd %%xmm10, %%xmm0;"
                         "sqrtpd %%xmm11, %%xmm0;"
                         "sqrtpd %%xmm12, %%xmm0;"
                         "sqrtpd %%xmm13, %%xmm0;"
                         "sqrtpd %%xmm14, %%xmm0;"
                         "sqrtpd %%xmm15, %%xmm0;"
                         "sqrtpd %%xmm8, %%xmm0;"
                         "sqrtpd %%xmm9, %%xmm0;"
                         "sqrtpd %%xmm10, %%xmm0;"
                         "sqrtpd %%xmm11, %%xmm0;"
                         "sqrtpd %%xmm12, %%xmm0;"
                         "sqrtpd %%xmm13, %%xmm0;"
                         "sqrtpd %%xmm14, %%xmm0;"
                         "sqrtpd %%xmm15, %%xmm0;"
                         "sqrtpd %%xmm8, %%xmm0;"
                         "sqrtpd %%xmm9, %%xmm0;"
                         "sqrtpd %%xmm10, %%xmm0;"
                         "sqrtpd %%xmm11, %%xmm0;"
                         "sqrtpd %%xmm12, %%xmm0;"
                         "sqrtpd %%xmm13, %%xmm0;"
                         "sqrtpd %%xmm14, %%xmm0;"
                         "sqrtpd %%xmm15, %%xmm0;"
                         "sqrtpd %%xmm8, %%xmm0;"
                         "sqrtpd %%xmm9, %%xmm0;"
                         "sqrtpd %%xmm10, %%xmm0;"
                         "sqrtpd %%xmm11, %%xmm0;"
                         "sqrtpd %%xmm12, %%xmm0;"
                         "sqrtpd %%xmm13, %%xmm0;"
                         "sqrtpd %%xmm14, %%xmm0;"
                         "sqrtpd %%xmm15, %%xmm0;"
                         "add $512,%%r9;"
                         "sub $1,%%r10;"
                         "jnz _skip_reset_sqrt_pd;" // reset buffer if the end is reached
                         "mov %%r14,%%r9;"          // restore addr
                         "mov %%r8,%%r10;"          // restore passes
                         "_skip_reset_sqrt_pd:"
                         "sub $32,%%r15;"
                         "jnz _work_loop_sqrt_pd;"

                         "mov %%r13,%%rcx;" // restore length
                         : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                         : "a"(addr), "b"(passes), "c"(length)
                         : "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "xmm0",
                           "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9",
                           "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"

    );
    ret = 2 * c;

    return ret;
}
