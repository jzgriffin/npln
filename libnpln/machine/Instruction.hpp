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

namespace libnpln::machine {

struct Instruction
{
    static auto decode(Word const w) -> Instruction;
    auto encode() const -> Word;

    Operator op;
    Operands args;
};

constexpr auto operator==(Instruction const& lhs, Instruction const& rhs)
    noexcept
{
    return lhs.op == rhs.op
        && lhs.args == rhs.args;
}

constexpr auto operator!=(Instruction const& lhs, Instruction const& rhs)
    noexcept
{
    return !(lhs == rhs);
}

}

template<>
struct fmt::formatter<libnpln::machine::Instruction>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Instruction const& value,
        FormatContext& context)
    {
        // This implementation of operand formatting is a hack.  Ideally, each
        // operand type could produce a fmt::format_arg_store containing all
        // of the named arguments provided by that operand.  This argument
        // store be applied to a format string with fmt::vformat.  However,
        // the type returned by fmt::arg only stores references to the
        // argument name and value.  Some of the operands format their values
        // as strings before instantiating the argument for that string.
        // There is no reasonable place to store these temporary strings with
        // a sufficient lifetime.  Thus, the fmt::format_arg_store refers to
        // invalid memory when returned.  The workaround is to apply those
        // temporary arguments in the same function as they are generated in,
        // as is done here.  The ideal solution is to extend fmt with an
        // owning named argument type, possibly renaming what is currently
        // called a named argument to be a named argument view.
        return std::visit([&](auto&& a) { return a.format(
            libnpln::machine::get_format_string(value.op), context); },
            value.args);
    }
};

#endif
