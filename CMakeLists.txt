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
    DESCRIPTION "A programming environment for PL/0 on CHIP-8"
    LANGUAGES CXX
)

include(CTest)

# Dependencies
find_package(fmt 6.0.0 REQUIRED)

find_package(Catch2 2.2.0 REQUIRED)
include(Catch)

find_package(CLI11 1.9.0 REQUIRED)

# libnpln target
add_library(libnpln
    libnpln/detail/ContainerCast.hpp
    libnpln/detail/cpp2b.hpp
    libnpln/detail/unreachable.hpp
    libnpln/machine/BitCodec.hpp
    libnpln/machine/BitCodecs.hpp
    libnpln/machine/DataUnits.hpp
    libnpln/machine/Display.cpp
    libnpln/machine/Display.hpp
    libnpln/machine/Fault.hpp
    libnpln/machine/Font.cpp
    libnpln/machine/Font.hpp
    libnpln/machine/Instruction.hpp
    libnpln/machine/Key.hpp
    libnpln/machine/Keys.cpp
    libnpln/machine/Keys.hpp
    libnpln/machine/Machine.cpp
    libnpln/machine/Machine.hpp
    libnpln/machine/Memory.cpp
    libnpln/machine/Memory.hpp
    libnpln/machine/Operand.hpp
    libnpln/machine/Operands.hpp
    libnpln/machine/Operator.hpp
    libnpln/machine/Register.hpp
    libnpln/machine/RegisterRange.hpp
    libnpln/machine/Registers.hpp
    libnpln/machine/Stack.hpp
    libnpln/utility/BitSetDifference.hpp
    libnpln/utility/FixedSizeStack.hpp
    libnpln/utility/HexDump.hpp
    libnpln/utility/Numeric.hpp
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
    libnpln/machine/Display.test.cpp
    libnpln/machine/Instruction.test.cpp
    libnpln/machine/Fault.test.cpp
    libnpln/machine/Font.test.cpp
    libnpln/machine/Key.test.cpp
    libnpln/machine/Keys.test.cpp
    libnpln/machine/Machine.test.cpp
    libnpln/machine/Memory.test.cpp
    libnpln/machine/Operand.test.cpp
    libnpln/machine/Operands.test.cpp
    libnpln/machine/Operator.test.cpp
    libnpln/machine/Register.test.cpp
    libnpln/machine/RegisterRange.test.cpp
    libnpln/machine/Registers.test.cpp
    libnpln/machine/Stack.test.cpp
    libnpln/utility/BitSetDifference.test.cpp
    libnpln/utility/FixedSizeStack.test.cpp
    libnpln/utility/Numeric.test.cpp
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
    CLI11::CLI11
)
