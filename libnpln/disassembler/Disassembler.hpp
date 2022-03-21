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

#ifndef LIBNPLN_DISASSEMBLER_DISASSEMBLER_HPP
#define LIBNPLN_DISASSEMBLER_DISASSEMBLER_HPP

#include <libnpln/disassembler/Table.hpp>
#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Instruction.hpp>
#include <libnpln/machine/Machine.hpp>

#include <gsl/span>

#include <optional>
#include <variant>
#include <vector>

namespace libnpln::disassembler {

class Disassembler
{
public:
    explicit Disassembler(gsl::span<machine::Byte const> program);

    [[nodiscard]] auto program() const noexcept -> gsl::span<machine::Byte const>
    {
        return program_;
    }

    [[nodiscard]] auto table() const noexcept -> Table const&
    {
        return table_;
    }

    auto run() -> Table const&;

private:
    [[nodiscard]] auto index() const -> gsl::index;
    [[nodiscard]] auto fetch() const -> std::optional<machine::Word>;

    auto step() -> bool;

    auto add_instruction(machine::Instruction const& instr) -> void;
    auto add_byte(machine::Byte byte) -> void;

    gsl::span<machine::Byte const> program_;
    machine::Address address_{machine::Machine::program_address};

    Table table_;
};

} // namespace libnpln::disassembler

#endif
