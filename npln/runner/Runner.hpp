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

#ifndef NPLN_RUNNER_RUNNER_HPP
#define NPLN_RUNNER_RUNNER_HPP

#include <libnpln/machine/Machine.hpp>

#include <chrono>

struct GLFWwindow;

namespace npln::runner {

struct Parameters;

class Runner
{
public:
    explicit Runner(Parameters const& params);
    Runner(Runner const&) = delete;
    Runner(Runner&&) noexcept = delete;
    ~Runner();

    auto operator=(Runner const&) -> Runner& = delete;
    auto operator=(Runner&&) noexcept -> Runner& = delete;

    auto run() -> int;

private:
    using FrameClock = std::chrono::high_resolution_clock;

    static auto install_error_callback() -> void;
    auto create_window() -> void;
    auto install_window_callbacks() -> void;

    auto process_key(int key, int scan_code, int action, int mods) -> void;
    auto update(FrameClock::duration const& frame_time) -> void;
    auto render() -> void;

    libnpln::machine::Machine machine;

    GLFWwindow* window = nullptr;
};

} // namespace npln::runner

#endif
