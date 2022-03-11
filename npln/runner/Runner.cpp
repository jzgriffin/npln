// Copyright 2020-2022 Jeremiah Griffin
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#define GLFW_INCLUDE_NONE

#include <npln/runner/Runner.hpp>

#include <npln/runner/Parameters.hpp>

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glad/glad.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace npln::runner {

Runner::Runner(Parameters const& params)
{
    using namespace libnpln::machine;
    if (!load_into_memory(params.path, machine.memory(), Machine::program_address)) {
        throw std::runtime_error{
            fmt::format("Unable to load program {} into memory", params.path.c_str())};
    }

    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error{"Unable to initialize GLFW"};
    }

    install_error_callback();
    create_window();
}

auto Runner::install_error_callback() -> void
{
    glfwSetErrorCallback([](int error, char const* desc) {
        std::cerr << "Error: GLFW reported code " << error << ": " << desc << '\n';
    });
}

auto Runner::create_window() -> void
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(1280, 720, "npln", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error{"Unable to create window"};
    }
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    // Interfacing with this C API requires reinterpret_cast.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        throw std::runtime_error{"Unable to initialize GLAD"};
    }

    install_window_callbacks();
}

auto Runner::install_window_callbacks() -> void
{
    glfwSetKeyCallback(
        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        window, [](GLFWwindow* window, int key, int scan_code, int action, int mods) {
            auto* self = static_cast<Runner*>(glfwGetWindowUserPointer(window));
            if (self != nullptr) {
                self->process_key(key, scan_code, action, mods);
            }
        });
}

Runner::~Runner()
{
    glfwTerminate();
}

auto Runner::run() -> int
{
    auto frame_time = FrameClock::duration{};
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        auto const start_time = FrameClock::now();

        glfwPollEvents();
        update(frame_time);
        render();
        glfwSwapBuffers(window);

        frame_time = FrameClock::now() - start_time;
    }
    return EXIT_SUCCESS;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto Runner::process_key(int key, int scan_code, int action, int mods) -> void
{
    (void)scan_code;
    (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

auto Runner::update(FrameClock::duration const& frame_time) -> void {}

auto Runner::render() -> void
{
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace npln::runner
