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

#ifndef LIBNPLN_MACHINE_REGISTERS_HPP
#define LIBNPLN_MACHINE_REGISTERS_HPP

#include <libnpln/detail/unreachable.hpp>
#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Register.hpp>

namespace libnpln::machine {

struct Registers
{
    constexpr auto operator[](Register const r) noexcept -> Byte&
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

        LIBNPLN_DETAIL_UNREACHABLE
    }

    constexpr auto operator[](Register const r) const noexcept -> Byte const&
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

        LIBNPLN_DETAIL_UNREACHABLE
    }

    // General-purpose
    Byte v0;
    Byte v1;
    Byte v2;
    Byte v3;
    Byte v4;
    Byte v5;
    Byte v6;
    Byte v7;
    Byte v8;
    Byte v9;
    Byte va;
    Byte vb;
    Byte vc;
    Byte vd;
    Byte ve;
    Byte vf;

    // Timer
    Byte dt;
    Byte st;

    // Index
    Word i;
};

}

#endif
