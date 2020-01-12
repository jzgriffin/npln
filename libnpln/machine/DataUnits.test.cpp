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

#include <libnpln/machine/DataUnits.hpp>

#include <catch2/catch.hpp>

#include <limits>

using namespace libnpln::machine;

TEST_CASE("Data unit types are of acceptable widths", "[machine][dataunits]")
{
    SECTION("Words are 16 bits wide")
    {
        REQUIRE(std::numeric_limits<Word>::digits == 16);
    }

    SECTION("Addresses are at least 12 bits wide")
    {
        REQUIRE(std::numeric_limits<Address>::digits >= 12);
    }

    SECTION("Bytes are 8 bits wide")
    {
        REQUIRE(std::numeric_limits<Byte>::digits == 8);
    }

    SECTION("Nibbles are at least 4 bits wide")
    {
        REQUIRE(std::numeric_limits<Nibble>::digits >= 4);
    }
}

TEST_CASE("Words can be constructed from bytes", "[machine][dataunits]")
{
    REQUIRE(make_word(0x12, 0x34) == 0x1234);
}
