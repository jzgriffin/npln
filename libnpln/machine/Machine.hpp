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
#include <gsl/gsl>

#include <optional>
#include <random>

namespace libnpln::machine {

class Machine
{
private:
    // This must be declared prior to the memory reference so that the pointer
    // is initialized in time for the reference to be made.
    gsl::not_null<std::unique_ptr<Memory>> const memory_;

public:
    Machine();
    Machine(Machine const& other);
    Machine(Machine&& other);

    auto operator=(Machine const& other) -> Machine&;
    auto operator=(Machine&& other) noexcept -> Machine&;

    auto operator==(Machine const& rhs) const noexcept
    {
        // Compare display and memory last because they are expensive to
        // compare.
        return fault == rhs.fault && program_counter == rhs.program_counter
            && registers == rhs.registers && stack == rhs.stack
            && keys == rhs.keys && display == rhs.display
            && memory == rhs.memory;
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
    Memory& memory;
    Keys keys;
    Display display;

    std::size_t master_clock_rate = 120; // Hz
    static constexpr std::size_t delay_clock_rate = 60; // Hz
    static constexpr std::size_t sound_clock_rate = 60; // Hz

    std::default_random_engine random_engine{std::random_device{}()};

    static constexpr Address font_address = 0x100;
    static constexpr Address program_address = 0x200;

private:
    using Result = std::optional<Fault::Type>;

    auto fetch() noexcept -> std::optional<Word>;
    auto execute(Instruction const& instr) noexcept -> Result;
    auto execute_cls() noexcept -> Result;
    auto execute_ret() noexcept -> Result;
    auto execute_jmp_a(AOperands const& args) noexcept -> Result;
    auto execute_call_a(AOperands const& args) noexcept -> Result;
    auto execute_seq_v_b(VBOperands const& args) noexcept -> Result;
    auto execute_sne_v_b(VBOperands const& args) noexcept -> Result;
    auto execute_seq_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_mov_v_b(VBOperands const& args) noexcept -> Result;
    auto execute_add_v_b(VBOperands const& args) noexcept -> Result;
    auto execute_mov_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_or_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_and_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_xor_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_add_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_sub_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_shr_v(VOperands const& args) noexcept -> Result;
    auto execute_subn_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_shl_v(VOperands const& args) noexcept -> Result;
    auto execute_sne_v_v(VVOperands const& args) noexcept -> Result;
    auto execute_mov_i_a(AOperands const& args) noexcept -> Result;
    auto execute_jmp_v0_a(AOperands const& args) noexcept -> Result;
    auto execute_rnd_v_b(VBOperands const& args) noexcept -> Result;
    auto execute_drw_v_v_n(VVNOperands const& args) noexcept -> Result;
    auto execute_skp_v(VOperands const& args) noexcept -> Result;
    auto execute_sknp_v(VOperands const& args) noexcept -> Result;
    auto execute_mov_v_dt(VOperands const& args) noexcept -> Result;
    auto execute_wkp_v(VOperands const& args) noexcept -> Result;
    auto execute_mov_dt_v(VOperands const& args) noexcept -> Result;
    auto execute_mov_st_v(VOperands const& args) noexcept -> Result;
    auto execute_add_i_v(VOperands const& args) noexcept -> Result;
    auto execute_font_v(VOperands const& args) noexcept -> Result;
    auto execute_bcd_v(VOperands const& args) noexcept -> Result;
    auto execute_mov_ii_v(VOperands const& args) noexcept -> Result;
    auto execute_mov_v_ii(VOperands const& args) noexcept -> Result;

    // These counters represent the number of master cycles since the last
    // decrement of the respective timer register.
    std::size_t delay_cycles = 0;
    std::size_t sound_cycles = 0;
};

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Machine>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Machine const& value, FormatContext& context)
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
            value.program_counter, value.registers, value.stack,
            libnpln::utility::to_hex_dump(value.memory), value.keys,
            value.display);
    }
};

#endif
