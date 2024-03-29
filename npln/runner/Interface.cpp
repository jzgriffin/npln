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

#include <npln/runner/Interface.hpp>

#include <npln/runner/Parameters.hpp>
#include <npln/runner/Runner.hpp>

#include <CLI/App.hpp>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <exception>
#include <typeinfo>

namespace npln::runner {

auto install_interface(CLI::App& app, Parameters& params) -> CLI::App*
{
    auto* run_app = app.add_subcommand("run", "Run a CHIP-8 executable");
    run_app->add_option("path", params.path, "Path to the executable file to run")->required();
    run_app->final_callback([&params]() {
        try {
            Runner{params}.run();
        }
        catch (std::exception const& e) {
            spdlog::error(
                "Uncaught exception of type {} in runner: {}", typeid(e).name(), e.what());
            throw CLI::RuntimeError{EXIT_FAILURE};
        }
    });
    return run_app;
}

} // namespace npln::runner
