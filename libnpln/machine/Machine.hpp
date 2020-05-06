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

#ifndef LIBNPLN_MACHINE_MACHINE_HPP
#define LIBNPLN_MACHINE_MACHINE_HPP

#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Display.hpp>
#include <libnpln/machine/Fault.hpp>
#include <libnpln/machine/Instruction.hpp>
#include <libnpln/machine/Keys.hpp>
#include <libnpln/machine/Memory.hpp>
#include <libnpln/machine/Registers.hpp>
#include <libnpln/machine/Stack.hpp>
#include <libnpln/utility/HexDump.hpp>

#include <fmt/format.h>

#include <optional>

namespace libnpln::machine {

struct Machine
{
    auto operator==(Machine const& rhs) const noexcept
    {
        return fault == rhs.fault
            && program_counter == rhs.program_counter
            && registers == rhs.registers
            && stack == rhs.stack
            && memory == rhs.memory
            && keys == rhs.keys
            && display == rhs.display;
    }

    auto operator!=(Machine const& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    auto cycle() noexcept -> bool;

    std::optional<Fault> fault;
    Address program_counter{program_address};
    Registers registers;
    Stack stack;
    Memory memory{};
    Keys keys;
    Display display;

private:
    using Result = std::optional<Fault::Type>;

    auto fetch() noexcept -> std::optional<Word>;
    auto execute(Address const pc, Instruction const& instr) noexcept -> Result;
    auto execute_cls(Address const pc, NullaryOperands const& args) noexcept -> Result;
    auto execute_ret(Address const pc, NullaryOperands const& args) noexcept -> Result;
    auto execute_jmp_a(Address const pc, AOperands const& args) noexcept -> Result;
    auto execute_call_a(Address const pc, AOperands const& args) noexcept -> Result;
    auto execute_seq_v_b(Address const pc, VBOperands const& args) noexcept -> Result;
    auto execute_sne_v_b(Address const pc, VBOperands const& args) noexcept -> Result;
    auto execute_seq_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_mov_v_b(Address const pc, VBOperands const& args) noexcept -> Result;
    auto execute_add_v_b(Address const pc, VBOperands const& args) noexcept -> Result;
    auto execute_mov_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_or_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_and_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_xor_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_add_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_sub_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_shr_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_subn_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_shl_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_sne_v_v(Address const pc, VVOperands const& args) noexcept -> Result;
    auto execute_mov_i_a(Address const pc, AOperands const& args) noexcept -> Result;
    auto execute_jmp_v0_a(Address const pc, AOperands const& args) noexcept -> Result;
    auto execute_rnd_v_b(Address const pc, VBOperands const& args) noexcept -> Result;
    auto execute_drw_v_v_n(Address const pc, VVNOperands const& args) noexcept -> Result;
    auto execute_skp_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_sknp_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_mov_v_dt(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_wkp_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_mov_dt_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_mov_st_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_add_i_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_font_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_bcd_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_mov_ii_v(Address const pc, VOperands const& args) noexcept -> Result;
    auto execute_mov_v_ii(Address const pc, VOperands const& args) noexcept -> Result;
};

}

template<>
struct fmt::formatter<libnpln::machine::Machine>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Machine const& value,
        FormatContext& context)
    {
        return format_to(context.out(),
            "fault: {}\n"
            "program counter: {:3X}h\n"
            "registers:\n{}\n"
            "stack: {{{}}}\n"
            "memory:\n{}\n"
            "keys: {{{}}}\n"
            "display:\n{}",
            value.fault == std::nullopt ? "none" : to_string(*value.fault),
            value.program_counter,
            value.registers,
            value.stack,
            libnpln::utility::to_hex_dump(value.memory),
            value.keys,
            value.display);
    }
};

#endif
