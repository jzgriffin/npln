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

#ifndef LIBNPLN_MACHINE_FAULT_HPP
#define LIBNPLN_MACHINE_FAULT_HPP

#include <libnpln/detail/unreachable.hpp>
#include <libnpln/machine/DataUnits.hpp>

#include <fmt/format.h>

#include <string_view>

namespace libnpln::machine {

struct Fault
{
    enum class Type
    {
        invalid_address,
        invalid_instruction,
        invalid_pixel,
        empty_stack,
        full_stack,
    };

    Type type;
    Address address;
};

constexpr auto operator==(Fault const& lhs, Fault const& rhs) noexcept
{
    return lhs.type == rhs.type
        && lhs.address == rhs.address;
}

constexpr auto operator!=(Fault const& lhs, Fault const& rhs)
    noexcept
{
    return !(lhs == rhs);
}

constexpr auto get_name(Fault::Type const t) -> std::string_view
{
    switch (t) {
        case Fault::Type::invalid_address: return "invalid_address";
        case Fault::Type::invalid_instruction: return "invalid_instruction";
        case Fault::Type::invalid_pixel: return "invalid_pixel";
        case Fault::Type::empty_stack: return "empty_stack";
        case Fault::Type::full_stack: return "full_stack";
    }

    LIBNPLN_DETAIL_UNREACHABLE
}

}

template<>
struct fmt::formatter<libnpln::machine::Fault::Type>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Fault::Type const& value,
        FormatContext& context)
    {
        return format_to(context.out(), "{}",
            libnpln::machine::get_name(value));
    }
};

template<>
struct fmt::formatter<libnpln::machine::Fault>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Fault const& value,
        FormatContext& context)
    {
        return format_to(context.out(), "{}@{:03X}h",
            value.type, value.address);
    }
};

#endif
