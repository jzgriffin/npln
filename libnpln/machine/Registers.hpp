// Copyright 2020-2021 Jeremiah Griffin
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

#ifndef LIBNPLN_MACHINE_REGISTERS_HPP
#define LIBNPLN_MACHINE_REGISTERS_HPP

#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Register.hpp>

#include <fmt/format.h>

#include <stdexcept>

namespace libnpln::machine {

struct Registers
{
    constexpr auto operator==(Registers const& rhs) const noexcept
    {
        return v0 == rhs.v0 && v1 == rhs.v1 && v2 == rhs.v2 && v3 == rhs.v3 && v4 == rhs.v4
            && v5 == rhs.v5 && v6 == rhs.v6 && v7 == rhs.v7 && v8 == rhs.v8 && v9 == rhs.v9
            && va == rhs.va && vb == rhs.vb && vc == rhs.vc && vd == rhs.vd && ve == rhs.ve
            && vf == rhs.vf && dt == rhs.dt && st == rhs.st && i == rhs.i;
    }

    constexpr auto operator!=(Registers const& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    constexpr auto operator[](Register const r) -> Byte&
    {
        switch (r) {
        case Register::v0: return v0;
        case Register::v1: return v1;
        case Register::v2: return v2;
        case Register::v3: return v3;
        case Register::v4: return v4;
        case Register::v5: return v5;
        case Register::v6: return v6;
        case Register::v7: return v7;
        case Register::v8: return v8;
        case Register::v9: return v9;
        case Register::va: return va;
        case Register::vb: return vb;
        case Register::vc: return vc;
        case Register::vd: return vd;
        case Register::ve: return ve;
        case Register::vf: return vf;
        }

        throw std::out_of_range("Unknown Register in Registers::operator[]");
    }

    constexpr auto operator[](Register const r) const -> Byte const&
    {
        switch (r) {
        case Register::v0: return v0;
        case Register::v1: return v1;
        case Register::v2: return v2;
        case Register::v3: return v3;
        case Register::v4: return v4;
        case Register::v5: return v5;
        case Register::v6: return v6;
        case Register::v7: return v7;
        case Register::v8: return v8;
        case Register::v9: return v9;
        case Register::va: return va;
        case Register::vb: return vb;
        case Register::vc: return vc;
        case Register::vd: return vd;
        case Register::ve: return ve;
        case Register::vf: return vf;
        }

        throw std::out_of_range("Unknown Register in Registers::operator[]");
    }

    // General-purpose
    Byte v0 = 0x00;
    Byte v1 = 0x00;
    Byte v2 = 0x00;
    Byte v3 = 0x00;
    Byte v4 = 0x00;
    Byte v5 = 0x00;
    Byte v6 = 0x00;
    Byte v7 = 0x00;
    Byte v8 = 0x00;
    Byte v9 = 0x00;
    Byte va = 0x00;
    Byte vb = 0x00;
    Byte vc = 0x00;
    Byte vd = 0x00;
    Byte ve = 0x00;
    Byte vf = 0x00;

    // Timer
    Byte dt = 0x00;
    Byte st = 0x00;

    // Index
    Word i = 0x0000;
};

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Registers>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Registers const& value, FormatContext& context)
    {
        return format_to(context.out(),
            "v0: {:02X}h, v1: {:02X}h, v2: {:02X}h, v3: {:02X}h,\n"
            "v4: {:02X}h, v5: {:02X}h, v6: {:02X}h, v7: {:02X}h,\n"
            "v8: {:02X}h, v9: {:02X}h, va: {:02X}h, vb: {:02X}h,\n"
            "vc: {:02X}h, vd: {:02X}h, ve: {:02X}h, vf: {:02X}h,\n"
            "dt: {:02X}h, st: {:02X}h, i: {:03X}h",
            value.v0, value.v1, value.v2, value.v3, value.v4, value.v5, value.v6, value.v7,
            value.v8, value.v9, value.va, value.vb, value.vc, value.vd, value.ve, value.vf,
            value.dt, value.st, value.i);
    }
};

#endif
