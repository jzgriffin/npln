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

#include <npln/Build.hpp>

#ifdef NPLN_BUILD_DISASSEMBLER
#    include <npln/disassembler/Interface.hpp>
#    include <npln/disassembler/Parameters.hpp>
#endif

#ifdef NPLN_BUILD_RUNNER
#    include <npln/runner/Interface.hpp>
#    include <npln/runner/Parameters.hpp>
#endif

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include <cstdlib>

auto main(int argc, char** argv) -> int
{
    CLI::App app{"PL/0 on CHIP-8 programming environment", "npln"};
    app.require_subcommand();

#ifdef NPLN_BUILD_DISASSEMBLER
    npln::disassembler::Parameters disassembler_params;
    npln::disassembler::install_interface(app, disassembler_params);
#endif

#ifdef NPLN_BUILD_RUNNER
    npln::runner::Parameters runner_params;
    npln::runner::install_interface(app, runner_params);
#endif

    try {
        app.parse(argc, argv);
    }
    catch (CLI::ParseError const& e) {
        return app.exit(e);
    }

    return EXIT_SUCCESS;
}
