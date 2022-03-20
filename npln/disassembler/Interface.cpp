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

#include <npln/disassembler/Interface.hpp>

#include <npln/disassembler/Disassembler.hpp>
#include <npln/disassembler/Parameters.hpp>

#include <CLI/App.hpp>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <exception>
#include <typeinfo>

namespace npln::disassembler {

auto install_interface(CLI::App& app, Parameters& params) -> CLI::App*
{
    auto* run_app = app.add_subcommand("disassemble", "Disassemble a CHIP-8 executable");
    run_app->add_flag("-a,!-A,--address,!--no-address", params.include_address,
        "Include the address of each disassembly item");
    run_app->add_flag("-c,!-C,--opcode,!--no-opcode", params.include_opcode,
        "Include the opcode of each disassembly item");
    run_app->add_option("-o,--output", params.output_path, "Path to the listing output file");
    run_app->add_option("input", params.input_path, "Path to the program input file")->required();
    run_app->final_callback([&params]() {
        try {
            Disassembler{params}.run();
        }
        catch (std::exception const& e) {
            spdlog::error(
                "Uncaught exception of type {} in disassembler: {}", typeid(e).name(), e.what());
            throw CLI::RuntimeError{EXIT_FAILURE};
        }
    });
    return run_app;
}

} // namespace npln::disassembler
