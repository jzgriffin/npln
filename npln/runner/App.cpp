// Copyright 2020 Jeremiah Griffin
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

#include <npln/runner/App.hpp>

#include <CLI/App.hpp>
#include <GLFW/glfw3.h>
#include <scope_guard.hpp>

#include <iostream>
#include <cstdlib>

namespace npln::runner {

App::App(CLI::App& app)
{
    auto run_app = app.add_subcommand("run", "Run a CHIP-8 executable");
    run_app->add_option("path", path, "Path to the executable file to run")
        ->required();
    run_app->final_callback([this]() {
        throw CLI::RuntimeError{run()};
    });
}

auto App::run() -> int
{
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    auto glfw_guard = sg::make_scope_guard([] { glfwTerminate(); });

    glfwSetErrorCallback([](int error, char const* desc) {
        std::cerr << "Error: GLFW reported code " << error << ": " << desc << '\n';
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    auto window = glfwCreateWindow(1280, 720, "npln", nullptr, nullptr);
    if (window == nullptr) {
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    return EXIT_SUCCESS;
}

}
