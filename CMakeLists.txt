# Copyright 2020-2022 Jeremiah Griffin
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
    LANGUAGES C CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Require C++17 without extensions.  These variables inform the target
# properties.  By explicitly requiring C++17, rather than simply setting
# the language version with the cxx_std_17 compiler feature, CMake will
# always add the appropriate compiler flag.  This ensures that the flag
# is present in compile_commands.json.  This is important when the
# compiler, such as GCC, defaults to C++17 but the language server, such
# as clangd, does not.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(NPLN_BUILD_RUNNER "Build the runner graphical interface" TRUE)
if(NPLN_BUILD_RUNNER)
    set(NPLN_BUILD_RENDERER ON)
endif()

# Dependencies
include(${CMAKE_CURRENT_BINARY_DIR}/conan_paths.cmake)

find_package(CLI11 REQUIRED)
find_package(EnumFlags REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

if(NPLN_BUILD_RUNNER)
    find_package(glfw3 REQUIRED)
    find_package(imgui REQUIRED)
endif()

if(NPLN_BUILD_RENDERER)
    find_package(glm REQUIRED)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    find_package(Catch2 REQUIRED)
    include(Catch)
endif()

# Sub-project dependencies
include(cmake/subproject/frequencypp.cmake)
include(cmake/subproject/gsl.cmake)

if(NPLN_BUILD_RENDERER)
    include(cmake/subproject/glbinding.cmake)
    include(cmake/subproject/globjects.cmake)  # Must come after glbinding
endif()

# libnpln target
add_library(libnpln
    libnpln/detail/ContainerCast.hpp
    libnpln/detail/Overload.hpp
    libnpln/detail/cpp2b.hpp
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
    EnumFlags::EnumFlags
    Microsoft.GSL::GSL
    fmt::fmt
    frequencypp::frequencypp
    spdlog::spdlog
)
target_include_directories(libnpln
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:>
)

# libnpln tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
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
endif()

# npln target
if(NPLN_BUILD_RUNNER)
    set(NPLN_BUILD_RENDERER ON)
    set(npln_RUNNER_SOURCE
        ${CMAKE_CURRENT_BINARY_DIR}/conf/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/conf/imgui_impl_glfw.h
        ${CMAKE_CURRENT_BINARY_DIR}/conf/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/conf/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_BINARY_DIR}/conf/imgui_impl_opengl3_loader.h
        npln/runner/GlfwError.cpp
        npln/runner/GlfwError.hpp
        npln/runner/GlfwLibrary.cpp
        npln/runner/GlfwLibrary.hpp
        npln/runner/Interface.cpp
        npln/runner/Interface.hpp
        npln/runner/Parameters.hpp
        npln/runner/Runner.cpp
        npln/runner/Runner.hpp
    )
    set(npln_RUNNER_LIBRARIES
        glfw
        imgui::imgui
    )
endif()
if(NPLN_BUILD_RENDERER)
    set(npln_RENDERER_SOURCE
        npln/renderer/DisplayTexture.cpp
        npln/renderer/DisplayTexture.hpp
    )
    set(npln_RENDERER_LIBRARIES
        glbinding::glbinding
        glm::glm
        globjects::globjects
    )
endif()
configure_file(
    npln/Build.hpp.in
    conf/npln/Build.hpp
    @ONLY
)
add_executable(npln
    ${CMAKE_CURRENT_BINARY_DIR}/conf/npln/Build.hpp
    ${npln_RENDERER_SOURCE}
    ${npln_RUNNER_SOURCE}
    npln/main.cpp
)
target_link_libraries(npln
    libnpln
    CLI11::CLI11
    ${npln_RENDERER_LIBRARIES}
    ${npln_RUNNER_LIBRARIES}
)
target_include_directories(npln
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/conf>
)
target_compile_definitions(npln
    PRIVATE
    GLFW_INCLUDE_NONE
)
