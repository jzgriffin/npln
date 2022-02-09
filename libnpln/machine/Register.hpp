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

#ifndef LIBNPLN_MACHINE_REGISTER_HPP
#define LIBNPLN_MACHINE_REGISTER_HPP

#include <libnpln/machine/DataUnits.hpp>

#include <fmt/format.h>

#include <stdexcept>
#include <string_view>

namespace libnpln::machine {

enum class Register : Byte
{
    v0 = 0x0,
    v1 = 0x1,
    v2 = 0x2,
    v3 = 0x3,
    v4 = 0x4,
    v5 = 0x5,
    v6 = 0x6,
    v7 = 0x7,
    v8 = 0x8,
    v9 = 0x9,
    va = 0xA,
    vb = 0xB,
    vc = 0xC,
    vd = 0xD,
    ve = 0xE,
    vf = 0xF,
};

constexpr auto get_name(Register const r) -> std::string_view
{
    switch (r) {
    case Register::v0: return "V0";
    case Register::v1: return "V1";
    case Register::v2: return "V2";
    case Register::v3: return "V3";
    case Register::v4: return "V4";
    case Register::v5: return "V5";
    case Register::v6: return "V6";
    case Register::v7: return "V7";
    case Register::v8: return "V8";
    case Register::v9: return "V9";
    case Register::va: return "VA";
    case Register::vb: return "VB";
    case Register::vc: return "VC";
    case Register::vd: return "VD";
    case Register::ve: return "VE";
    case Register::vf: return "VF";
    }

    throw std::out_of_range("Unknown Register in get_name");
}

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Register>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Register const& value, FormatContext& context)
    {
        return format_to(context.out(), "{}", libnpln::machine::get_name(value));
    }
};

#endif
