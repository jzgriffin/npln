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

#ifndef NPLN_RUNNER_GLFWERROR_HPP
#define NPLN_RUNNER_GLFWERROR_HPP

#include <stdexcept>
#include <string>

namespace npln::runner {

class GlfwError : public std::runtime_error
{
public:
    GlfwError(int code, std::string_view message);
    GlfwError(GlfwError const&) = default;
    GlfwError(GlfwError&&) noexcept = default;
    ~GlfwError() override = default;

    auto operator=(GlfwError const&) -> GlfwError& = default;
    auto operator=(GlfwError&&) noexcept -> GlfwError& = default;

    [[nodiscard]] auto code() const noexcept -> int
    {
        return code_;
    }
    [[nodiscard]] auto message() const noexcept -> std::string const&
    {
        return message_;
    }

private:
    int code_;
    std::string message_;
};

} // namespace npln::runner

#endif // NPLN_RUNNER_GLFWERROR_HPP
