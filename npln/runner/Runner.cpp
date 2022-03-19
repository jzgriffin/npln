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

#include <npln/runner/Runner.hpp>

#include <npln/renderer/DisplayTexture.hpp>
#include <npln/runner/GlfwError.hpp>
#include <npln/runner/Parameters.hpp>

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glbinding/gl/gl.h>
#include <globjects/TextureHandle.h>
#include <globjects/globjects.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace npln::runner {

Runner::Runner(Parameters const& params)
{
    using namespace libnpln::machine;
    if (!load_into_memory(params.path, machine.memory(), Machine::program_address)) {
        throw std::runtime_error{
            fmt::format("Unable to load program {} into memory", params.path.c_str())};
    }

    install_error_callback();
    create_window();
    initialize_globjects();
    initialize_imgui();

    display_texture_ = std::make_unique<renderer::DisplayTexture>(machine.display());
}

Runner::~Runner()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

auto Runner::install_error_callback() -> void
{
    glfwSetErrorCallback([](int code, char const* message) { throw GlfwError{code, message}; });
}

auto Runner::create_window() -> void
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    window = glfwCreateWindow(1280, 720, "npln", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error{"Unable to create window"};
    }
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);

    install_window_callbacks();
}

auto Runner::install_window_callbacks() -> void
{
    glfwSetFramebufferSizeCallback(
        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        window, [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<Runner*>(glfwGetWindowUserPointer(window));
            if (self != nullptr) {
                self->process_framebuffer_size(width, height);
            }
        });

    glfwSetKeyCallback(
        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        window, [](GLFWwindow* window, int key, int scan_code, int action, int mods) {
            auto* self = static_cast<Runner*>(glfwGetWindowUserPointer(window));
            if (self != nullptr) {
                self->process_key(key, scan_code, action, mods);
            }
        });
}

// NOLINTNEXTLINE
auto Runner::initialize_globjects() -> void
{
    globjects::init(glfwGetProcAddress);
}

auto Runner::initialize_imgui() -> void
{
    IMGUI_CHECKVERSION();
    if (ImGui::CreateContext() == nullptr) {
        throw std::runtime_error{"Unable to create ImGui context"};
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        throw std::runtime_error{"Unable to initialize ImGui GLFW OpenGL implementation"};
    }
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        throw std::runtime_error{"Unable to initialize ImGui OpenGL 3 implementation"};
    }
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

// NOLINTNEXTLINE
auto Runner::process_framebuffer_size(int width, int height) -> void
{
    gl::glViewport(0, 0, width, height);
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

auto Runner::update(FrameClock::duration const& frame_time) -> void
{
    cycle_machine(frame_time);

    display_texture_->update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto Runner::render() -> void
{
    render_display();

    gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto Runner::render_display() -> void
{
    display_texture_->render();

    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
    ImGui::Begin("Display", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
    ImGui::Image(
        // NOLINTNEXTLINE
        reinterpret_cast<ImTextureID>(display_texture_->texture().textureHandle().handle()),
        ImGui::GetWindowSize());
    ImGui::End();
    ImGui::PopStyleVar(2);
}

auto Runner::cycle_machine(FrameClock::duration const& frame_time) -> void
{
    // Ensure that the machine cycles in real time regardless of the update rate.
    accumulated_frame_time += frame_time;
    const auto passed_cycles = accumulated_frame_time * machine.master_clock_rate();
    accumulated_frame_time -= frequencypp::duration_cast<FrameClock::duration>(
        passed_cycles * machine.master_clock_rate());
    for (std::decay_t<decltype(passed_cycles)> i = 0; i < passed_cycles; ++i) {
        machine.cycle();
    }
}

} // namespace npln::runner
