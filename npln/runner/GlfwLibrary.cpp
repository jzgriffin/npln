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

#include <npln/runner/GlfwLibrary.hpp>

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace npln::runner {

GlfwLibrary::GlfwLibrary()
{
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error{"Unable to initialize GLFW"};
    }
}

GlfwLibrary::~GlfwLibrary()
{
    glfwTerminate();
}

} // namespace npln::runner
