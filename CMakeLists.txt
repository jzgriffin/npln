# Copyright 2020 Jeremiah Griffin
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Project-wide
project(npln
    VERSION 0.1.0
    DESCRIPTION "A programming environment for CHIP-8 on PL/0"
    LANGUAGES CXX
)

include(CTest)

# Dependencies
find_package(fmt 6.1.0 REQUIRED)

find_package(Catch2 2.2.0 REQUIRED)
include(Catch)

# libnpln target
add_library(libnpln
    libnpln/detail/cpp2b.hpp
    libnpln/dummy.cpp
    libnpln/machine/BitCodec.hpp
    libnpln/machine/DataUnits.hpp
    libnpln/machine/Operator.hpp
    libnpln/machine/Register.hpp
)
target_compile_features(libnpln
    PUBLIC
    cxx_std_17
)
target_link_libraries(libnpln
    PUBLIC
    fmt::fmt
)
target_include_directories(libnpln
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:>
)

# libnpln tests
add_executable(test-libnpln
    libnpln/libnpln.test.cpp
    libnpln/machine/BitCodec.test.cpp
    libnpln/machine/DataUnits.test.cpp
    libnpln/machine/Operator.test.cpp
    libnpln/machine/Register.test.cpp
)
target_link_libraries(test-libnpln
    libnpln
    Catch2::Catch2
)
catch_discover_tests(test-libnpln)

# npln target
add_executable(npln
    npln/main.cpp
)
target_link_libraries(npln
    libnpln
)
