cmake_minimum_required(VERSION 3.11)
project(roco2_kernels)

include(CheckLanguage)

find_package(CBLAS REQUIRED)

add_subdirectory(lib/firestarter)
add_subdirectory(lib/penguinxx)

add_library(libroco2_kernels src/lib/kernel.cpp)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    message(STATUS "Building WITH aarch64 asm kernels.")
    target_sources(libroco2_kernels PRIVATE src/lib/asm_kernels_aarch64.cpp)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    message(STATUS "Building WITH x86_64 asm kernels.")
    target_sources(libroco2_kernels PRIVATE src/lib/asm_kernels_x86.cpp)
else()
    message(FATAL_ERROR "No ASM kernels for architecture: ${CMAKE_SYSTEM_PROCESSOR} YET!")
endif()

target_include_directories(libroco2_kernels PUBLIC include)

target_link_libraries(libroco2_kernels PUBLIC cblas Firestarter::payload Penguinxx::cpu)

set_property(TARGET libroco2_kernels PROPERTY CXX_STANDARD 17)

# Otherwise we might not be able to link to CBLAS due to missing Fortran symbols
if(CMAKE_C_COMPILER_ID STREQUAL "NVHPC")
    target_compile_options(libroco2_kernels INTERFACE -pgf90libs)
    target_link_options(libroco2_kernels INTERFACE -pgf90libs)
endif()

add_executable(kernel_runner src/kernel_runner.cpp)
target_link_libraries(kernel_runner libroco2_kernels)

check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    target_sources(kernel_runner PRIVATE src/roco2.cu)
    target_compile_definitions(kernel_runner PUBLIC HAVE_CUDA)
endif()

add_executable(frequency_sweeper src/frequency_sweeper.cpp)
target_link_libraries(frequency_sweeper libroco2_kernels)

add_executable(kernel_list src/tools/kernel_list.cpp)
target_link_libraries(kernel_list libroco2_kernels)
