cmake_minimum_required(VERSION 3.16)
project(mylib
    VERSION 0.1.0
    DESCRIPTION ""
    HOMEPAGE_URL "https://github.com/sigerror/upmem-cmake-template"
    LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

##############################################################################
# Configuration

set(MYLIB_NR_DPUS 16 CACHE STRING "Total number of DPUs available")
set(MYLIB_NR_TASKLETS 16 CACHE STRING "Number of tasklets per DPU (v1A: 24, v1B: 16)")
set(MYLIB_MIN_XFER_SZ 8 CACHE STRING "Minimum memory tranfer size (bytes)")

##############################################################################
# Dependencies

find_package(DPU 2025.1 REQUIRED)
find_package(OpenMP REQUIRED COMPONENTS CXX)

include(CPM)
if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.8.0")
endif()

##############################################################################
# Kernel binaries

include(ExternalProject)
externalproject_add(mylib_kernel
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/kernel
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/kernel
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_TOOLCHAIN_FILE=${DPU_TOOLCHAIN_FILE}
        -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
        -DNR_TASKLETS=${MYLIB_NR_TASKLETS}
    BUILD_ALWAYS TRUE)

##############################################################################
# Host library

add_subdirectory(mylib)

##############################################################################
# Tests

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
