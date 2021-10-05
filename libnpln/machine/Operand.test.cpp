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

#include <libnpln/machine/Operand.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("Operand codecs map to the correct bits", "[machine][operand]")
{
    SECTION("Address operand codec maps to the correct bits")
    {
        REQUIRE(AddressOperand::bit_width == 12);
        REQUIRE(AddressOperand::bit_offset == 0);
    }

    SECTION("Byte operand codec maps to the correct bits")
    {
        REQUIRE(ByteOperand::bit_width == 8);
        REQUIRE(ByteOperand::bit_offset == 0);
    }

    SECTION("Vx operand codec maps to the correct bits")
    {
        REQUIRE(VxOperand::bit_width == 4);
        REQUIRE(VxOperand::bit_offset == 8);
    }

    SECTION("Vy operand codec maps to the correct bits")
    {
        REQUIRE(VyOperand::bit_width == 4);
        REQUIRE(VyOperand::bit_offset == 4);
    }

    SECTION("Nibble operand codec maps to the correct bits")
    {
        REQUIRE(NibbleOperand::bit_width == 4);
        REQUIRE(NibbleOperand::bit_offset == 0);
    }
}
