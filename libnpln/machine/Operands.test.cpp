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

#include <libnpln/machine/Operands.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("Operands encode/decode correctly", "[machine][operands]")
{
    SECTION("Nullary")
    {
        auto const x = NullaryOperands{};
        REQUIRE(x.encode() == 0x0000);
        REQUIRE(x.decode(x.encode()) == x);
    }

    SECTION("Address")
    {
        auto const x = AOperands{0xABC};
        REQUIRE(x.encode() == 0x0ABC);
        REQUIRE(x.decode(x.encode()) == x);
    }

    SECTION("Register")
    {
        auto const x = VOperands{Register::va};
        REQUIRE(x.encode() == 0x0A00);
        REQUIRE(x.decode(x.encode()) == x);
    }

    SECTION("Register-byte")
    {
        auto const x = VBOperands{Register::vb, 0xCD};
        REQUIRE(x.encode() == 0x0BCD);
        REQUIRE(x.decode(x.encode()) == x);
    }

    SECTION("Register-register")
    {
        auto const x = VVOperands{Register::vc, Register::v4};
        REQUIRE(x.encode() == 0x0C40);
        REQUIRE(x.decode(x.encode()) == x);
    }

    SECTION("Register-register-nibble")
    {
        auto const x = VVNOperands{Register::v2, Register::vf, 0x7};
        REQUIRE(x.encode() == 0x02F7);
        REQUIRE(x.decode(x.encode()) == x);
    }
}
