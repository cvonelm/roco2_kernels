// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <cstdint>

uint64_t mulpd_kernel(double* buffer, uint64_t repeat);
uint64_t addpd_kernel(double* buffer, uint64_t repeat);

uint64_t sqrtss_kernel(float* buffer, uint64_t elems, uint64_t repeat);
uint64_t sqrtsd_kernel(double* buffer, uint64_t elems, uint64_t repeat);
uint64_t sqrtps_kernel(float* buffer, uint64_t elems, uint64_t repeat);
uint64_t sqrtpd_kernel(double* buffer, uint64_t elems, uint64_t repeat);
