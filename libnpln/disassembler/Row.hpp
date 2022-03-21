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

#ifndef LIBNPLN_DISASSEMBLER_ROW_HPP
#define LIBNPLN_DISASSEMBLER_ROW_HPP

#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Instruction.hpp>

#include <libnpln/detail/Overload.hpp>

#include <fmt/format.h>

#include <string>
#include <variant>

namespace libnpln::disassembler {

struct Row
{
    [[nodiscard]] constexpr auto data_width() const noexcept -> std::size_t
    {
        return std::visit(
            detail::overload{
                [](machine::Instruction const&) { return machine::Instruction::width; },
                [](machine::Byte b) { return sizeof b; },
            },
            data);
    }

    [[nodiscard]] constexpr auto end_address() const noexcept -> machine::Address
    {
        return address + data_width();
    }

    machine::Address address;
    std::variant<machine::Instruction, machine::Byte> data;
    std::string label;
};

// Determine whether the ranges of addresses spanned by two rows intersect.
constexpr auto intersects(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return lhs.end_address() > rhs.address && lhs.address < rhs.end_address();
}

// Determine whether an address intersects with the range of addresses spanned by a row.
constexpr auto intersects(Row const& lhs, machine::Address const rhs) noexcept -> bool
{
    return lhs.address <= rhs && rhs < lhs.end_address();
}

// Equality comparisons are member-wise.

constexpr auto operator==(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return lhs.address == rhs.address && lhs.data == rhs.data && lhs.label == rhs.label;
}

constexpr auto operator!=(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

// Ordering comparisons are address-wise, counting intersections as equality.

constexpr auto operator<(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return lhs.address < rhs.address && !intersects(lhs, rhs);
}

constexpr auto operator<(Row const& lhs, machine::Address const rhs) noexcept -> bool
{
    return lhs.address < rhs && !intersects(lhs, rhs);
}

constexpr auto operator<(machine::Address const lhs, Row const& rhs) noexcept -> bool
{
    return lhs < rhs.address && !intersects(rhs, lhs);
}

constexpr auto operator<=(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return lhs.address <= rhs.address || intersects(lhs, rhs);
}

constexpr auto operator<=(Row const& lhs, machine::Address const rhs) noexcept -> bool
{
    return lhs.address <= rhs || intersects(lhs, rhs);
}

constexpr auto operator<=(machine::Address const lhs, Row const& rhs) noexcept -> bool
{
    return lhs <= rhs.address || intersects(rhs, lhs);
}

constexpr auto operator>(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return lhs.address > rhs.address && !intersects(lhs, rhs);
}

constexpr auto operator>(Row const& lhs, machine::Address const rhs) noexcept -> bool
{
    return lhs.address > rhs && !intersects(lhs, rhs);
}

constexpr auto operator>(machine::Address const lhs, Row const& rhs) noexcept -> bool
{
    return lhs > rhs.address && !intersects(rhs, lhs);
}

constexpr auto operator>=(Row const& lhs, Row const& rhs) noexcept -> bool
{
    return lhs.address >= rhs.address || intersects(lhs, rhs);
}

constexpr auto operator>=(Row const& lhs, machine::Address const rhs) noexcept -> bool
{
    return lhs.address >= rhs || intersects(lhs, rhs);
}

constexpr auto operator>=(machine::Address const lhs, Row const& rhs) noexcept -> bool
{
    return lhs >= rhs.address || intersects(rhs, lhs);
}

} // namespace libnpln::disassembler

template<>
struct fmt::formatter<libnpln::disassembler::Row>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::disassembler::Row const& value, FormatContext& context)
    {
        return format_to(context.out(), "{}@{:03X}-{:03X}:{}", value.label, value.address,
            value.end_address(),
            std::visit(libnpln::detail::overload{
                           [](libnpln::machine::Instruction const& i) {
                               return fmt::format("Instruction({})", i);
                           },
                           [](libnpln::machine::Byte b) { return fmt::format("Byte({:02X})", b); },
                       },
                value.data));
    }
};

#endif
