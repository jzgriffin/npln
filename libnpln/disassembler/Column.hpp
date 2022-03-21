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

#ifndef LIBNPLN_DISASSEMBLER_COLUMN_HPP
#define LIBNPLN_DISASSEMBLER_COLUMN_HPP

#include <libnpln/detail/cpp2b.hpp>

#include <flags/flags.hpp>
#include <fmt/format.h>

#include <stdexcept>
#include <string_view>
#include <vector>

namespace libnpln::disassembler {

enum class Column
{
    address = 1U << 0U,
    opcode = 1U << 1U,
    label = 1U << 2U,
    instruction = 1U << 3U,
};

} // namespace libnpln::disassembler

ALLOW_FLAGS_FOR_ENUM(libnpln::disassembler::Column);

namespace libnpln::disassembler {

extern flags::flags<Column> const no_columns;
extern flags::flags<Column> const all_columns;

constexpr auto get_name(Column const c) -> std::string_view
{
    switch (c) {
    case Column::address: return "Address";
    case Column::opcode: return "Opcode";
    case Column::label: return "Label";
    case Column::instruction: return "Instruction";
    }

    throw std::out_of_range("Unknown Column in get_name");
}

} // namespace libnpln::disassembler

template<>
struct fmt::formatter<libnpln::disassembler::Column>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::disassembler::Column const& value, FormatContext& context)
    {
        return format_to(context.out(), "{}", libnpln::disassembler::get_name(value));
    }
};

#endif
