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

#ifndef LIBNPLN_MACHINE_INSTRUCTION_HPP
#define LIBNPLN_MACHINE_INSTRUCTION_HPP

#include <libnpln/machine/Operands.hpp>
#include <libnpln/machine/Operator.hpp>

#include <fmt/format.h>

#include <optional>

namespace libnpln::machine {

struct Instruction
{
    static constexpr auto decode(Word const w) noexcept -> std::optional<Instruction>
    {
        // The decode_* operations cascade until a matching decoding is found or the possible
        // decodings are exhausted, in which case a null optional is returned.  The decode_ffff
        // operation is the highest level of this cascade.
        return decode_ffff(w);
    }

    constexpr auto encode() const noexcept -> Word
    {
        return static_cast<Word>(op) | std::visit([](auto&& a) { return a.encode(); }, args);
    }

    static constexpr auto width = sizeof(Word);

    Operator op;
    Operands args;

private:
    static constexpr auto decode_f000(Word const w) noexcept -> std::optional<Instruction>
    {
        auto op = static_cast<Operator>(w & 0xF000);
        switch (op) {
        case Operator::jmp_a:
        case Operator::call_a:
        case Operator::mov_i_a:
        case Operator::jmp_v0_a: return {{op, AOperands::decode(w)}};
        case Operator::seq_v_b:
        case Operator::sne_v_b:
        case Operator::mov_v_b:
        case Operator::add_v_b:
        case Operator::rnd_v_b: return {{op, VBOperands::decode(w)}};
        case Operator::drw_v_v_n: return {{op, VVNOperands::decode(w)}};
        default: return std::nullopt;
        }
    }

    static constexpr auto decode_f00f(Word const w) noexcept -> std::optional<Instruction>
    {
        auto op = static_cast<Operator>(w & 0xF00F);
        switch (op) {
        case Operator::seq_v_v:
        case Operator::mov_v_v:
        case Operator::or_v_v:
        case Operator::and_v_v:
        case Operator::xor_v_v:
        case Operator::add_v_v:
        case Operator::sub_v_v:
        case Operator::subn_v_v:
        case Operator::sne_v_v: return {{op, VVOperands::decode(w)}};
        default: return decode_f000(w);
        }
    }

    static constexpr auto decode_f0ff(Word const w) noexcept -> std::optional<Instruction>
    {
        auto op = static_cast<Operator>(w & 0xF0FF);
        switch (op) {
        case Operator::shr_v:
        case Operator::shl_v:
        case Operator::skp_v:
        case Operator::sknp_v:
        case Operator::mov_v_dt:
        case Operator::wkp_v:
        case Operator::mov_dt_v:
        case Operator::mov_st_v:
        case Operator::add_i_v:
        case Operator::font_v:
        case Operator::bcd_v:
        case Operator::mov_ii_v:
        case Operator::mov_v_ii: return {{op, VOperands::decode(w)}};
        default: return decode_f00f(w);
        }
    }

    static constexpr auto decode_ffff(Word const w) noexcept -> std::optional<Instruction>
    {
        auto op = static_cast<Operator>(w);
        switch (op) {
        case Operator::cls:
        case Operator::ret: return {{op, NullaryOperands::decode(w)}};
        default: return decode_f0ff(w);
        }
    }
};

constexpr auto operator==(Instruction const& lhs, Instruction const& rhs) noexcept
{
    return lhs.op == rhs.op && lhs.args == rhs.args;
}

constexpr auto operator!=(Instruction const& lhs, Instruction const& rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Instruction>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Instruction const& value, FormatContext& context)
    {
        // This implementation of operand formatting is a hack.  Ideally, each operand type could
        // produce a fmt::format_arg_store containing all of the named arguments provided by that
        // operand.  This argument store be applied to a format string with fmt::vformat.  However,
        // the type returned by fmt::arg only stores references to the argument name and value.
        // Some of the operands format their values as strings before instantiating the argument for
        // that string.  There is no reasonable place to store these temporary strings with a
        // sufficient lifetime.  Thus, the fmt::format_arg_store refers to invalid memory when
        // returned.  The workaround is to apply those temporary arguments in the same function as
        // they are generated in, as is done here.  The ideal solution is to extend fmt with an
        // owning named argument type, possibly renaming what is currently called a named argument
        // to be a named argument view.
        return std::visit(
            [&](auto&& a) {
                return a.format(libnpln::machine::get_format_string(value.op), context);
            },
            value.args);
    }
};

#endif
