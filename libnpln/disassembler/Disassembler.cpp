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

#include <libnpln/disassembler/Disassembler.hpp>

#include <gsl/gsl>

namespace libnpln::disassembler {

Disassembler::Disassembler(gsl::span<machine::Byte const> const program) : program_{program} {}

auto Disassembler::run() -> Table const&
{
    if (table_.empty()) {
        step();
    }
    return table_;
}

auto Disassembler::index() const -> gsl::index
{
    return gsl::narrow<gsl::index>(address_) - machine::Machine::program_address;
}

auto Disassembler::fetch() const -> std::optional<machine::Word>
{
    auto const idx = index();
    if (idx < 0 || idx + 1 >= program_.size()) {
        return std::nullopt;
    }

    auto const high = gsl::at(program_, idx + 0);
    auto const low = gsl::at(program_, idx + 1);
    return machine::make_word(high, low); // Big-endian
}

auto Disassembler::step() -> bool
{
    using namespace machine;

    // Treat the current address as a data byte if it's the last byte in the program (fetch fails)
    // or it, combined with the following byte, does not form an instruction (decode fails).
    auto const w = fetch();
    auto const i = w != std::nullopt ? Instruction::decode(*w) : std::nullopt;
    if (i == std::nullopt) {
        auto const idx = index();
        if (idx < 0 || idx >= program_.size()) {
            return false;
        }

        auto const b = gsl::at(program_, idx);
        add_byte(b);
        address_ += sizeof b;
        return step();
    }

    add_instruction(*i);
    address_ += Instruction::width;
    return step();
}

auto Disassembler::add_instruction(machine::Instruction const& instr) -> void
{
    insert_row(table_, {address_, instr});
}

auto Disassembler::add_byte(machine::Byte byte) -> void
{
    insert_row(table_, {address_, byte});
}

} // namespace libnpln::disassembler
