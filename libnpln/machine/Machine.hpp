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
#include <fmt/ostream.h>
#include <frequencypp/frequency.hpp>

#include <optional>
#include <random>

namespace libnpln::machine {

class Machine
{
public:
    Machine();
    Machine(Machine const& other);
    Machine(Machine&& other) noexcept;
    ~Machine() = default;

    auto operator=(Machine const& other) -> Machine&;
    auto operator=(Machine&& other) noexcept -> Machine&;

    auto operator==(Machine const& rhs) const noexcept
    {
        // Compare display and memory last because they are expensive to compare.
        return fault_ == rhs.fault_ && program_counter_ == rhs.program_counter_
            && registers_ == rhs.registers_ && stack_ == rhs.stack_ && keys_ == rhs.keys_
            && display_ == rhs.display_ && *memory_ == *rhs.memory_;
    }

    auto operator!=(Machine const& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    auto cycle() -> bool;

    auto fault() noexcept -> std::optional<Fault>&
    {
        return fault_;
    }
    [[nodiscard]] auto fault() const noexcept -> std::optional<Fault> const&
    {
        return fault_;
    }
    auto program_counter() noexcept -> Address&
    {
        return program_counter_;
    }
    [[nodiscard]] auto program_counter() const noexcept -> Address const&
    {
        return program_counter_;
    }
    auto registers() noexcept -> Registers&
    {
        return registers_;
    }
    [[nodiscard]] auto registers() const noexcept -> Registers const&
    {
        return registers_;
    }
    auto stack() noexcept -> Stack&
    {
        return stack_;
    }
    [[nodiscard]] auto stack() const noexcept -> Stack const&
    {
        return stack_;
    }
    auto memory() noexcept -> Memory&
    {
        return *memory_;
    }
    [[nodiscard]] auto memory() const noexcept -> Memory const&
    {
        return *memory_;
    }
    auto keys() noexcept -> Keys&
    {
        return keys_;
    }
    [[nodiscard]] auto keys() const noexcept -> Keys const&
    {
        return keys_;
    }
    auto display() noexcept -> Display&
    {
        return display_;
    }
    [[nodiscard]] auto display() const noexcept -> Display const&
    {
        return display_;
    }

    auto master_clock_rate() noexcept -> frequencypp::hertz&
    {
        return master_clock_rate_;
    }
    [[nodiscard]] auto master_clock_rate() const noexcept -> frequencypp::hertz const&
    {
        return master_clock_rate_;
    }

    static constexpr frequencypp::hertz delay_clock_rate{60};
    static constexpr frequencypp::hertz sound_clock_rate{60};

    static constexpr Address font_address = 0x100;
    static constexpr Address program_address = 0x200;

private:
    using Result = std::optional<Fault::Type>;

    auto fetch() noexcept -> std::optional<Word>;
    auto execute(Instruction const& instr) -> Result;
    auto execute_cls() -> Result;
    auto execute_ret() -> Result;
    auto execute_jmp_a(AOperands const& args) -> Result;
    auto execute_call_a(AOperands const& args) -> Result;
    auto execute_seq_v_b(VBOperands const& args) -> Result;
    auto execute_sne_v_b(VBOperands const& args) -> Result;
    auto execute_seq_v_v(VVOperands const& args) -> Result;
    auto execute_mov_v_b(VBOperands const& args) -> Result;
    auto execute_add_v_b(VBOperands const& args) -> Result;
    auto execute_mov_v_v(VVOperands const& args) -> Result;
    auto execute_or_v_v(VVOperands const& args) -> Result;
    auto execute_and_v_v(VVOperands const& args) -> Result;
    auto execute_xor_v_v(VVOperands const& args) -> Result;
    auto execute_add_v_v(VVOperands const& args) -> Result;
    auto execute_sub_v_v(VVOperands const& args) -> Result;
    auto execute_shr_v(VOperands const& args) -> Result;
    auto execute_subn_v_v(VVOperands const& args) -> Result;
    auto execute_shl_v(VOperands const& args) -> Result;
    auto execute_sne_v_v(VVOperands const& args) -> Result;
    auto execute_mov_i_a(AOperands const& args) -> Result;
    auto execute_jmp_v0_a(AOperands const& args) -> Result;
    auto execute_rnd_v_b(VBOperands const& args) -> Result;
    auto execute_drw_v_v_n(VVNOperands const& args) -> Result;
    auto execute_skp_v(VOperands const& args) -> Result;
    auto execute_sknp_v(VOperands const& args) -> Result;
    auto execute_mov_v_dt(VOperands const& args) -> Result;
    auto execute_wkp_v(VOperands const& args) -> Result;
    auto execute_mov_dt_v(VOperands const& args) -> Result;
    auto execute_mov_st_v(VOperands const& args) -> Result;
    auto execute_add_i_v(VOperands const& args) -> Result;
    auto execute_font_v(VOperands const& args) -> Result;
    auto execute_bcd_v(VOperands const& args) -> Result;
    auto execute_mov_ii_v(VOperands const& args) -> Result;
    auto execute_mov_v_ii(VOperands const& args) -> Result;

    std::optional<Fault> fault_;
    Address program_counter_{program_address};
    Registers registers_;
    Stack stack_;
    std::unique_ptr<Memory> memory_;
    Keys keys_;
    Display display_;

    frequencypp::hertz master_clock_rate_{120};

    // These counters represent the number of master cycles since the last decrement of the
    // respective timer register.
    std::size_t delay_cycles = 0;
    std::size_t sound_cycles = 0;

    std::default_random_engine random_engine{std::random_device{}()};
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
            "master clock rate: {}\n"
            "program counter: {:3X}h\n"
            "registers:\n{}\n"
            "stack: {{{}}}\n"
            "memory:\n{}\n"
            "keys: {{{}}}\n"
            "display:\n{}",
            value.fault() == std::nullopt ? "none" : to_string(*value.fault()),
            value.master_clock_rate(), value.program_counter(), value.registers(), value.stack(),
            libnpln::utility::to_hex_dump(value.memory()), value.keys(), value.display());
    }
};

#endif
