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

#include <libnpln/machine/BitCodec.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("BitCodec produces the correct masks", "[machine][bitcodec]")
{
    REQUIRE(BitCodec<std::uint8_t, 4, 0>::bit_mask == 0b00001111);
    REQUIRE(BitCodec<std::uint8_t, 4, 1>::bit_mask == 0b00011110);
    REQUIRE(BitCodec<std::uint8_t, 4, 2>::bit_mask == 0b00111100);
    REQUIRE(BitCodec<std::uint8_t, 4, 3>::bit_mask == 0b01111000);
    REQUIRE(BitCodec<std::uint8_t, 4, 4>::bit_mask == 0b11110000);
}

TEST_CASE("BitCodec decodes into the lower bits", "[machine][bitcodec]")
{
    REQUIRE(BitCodec<std::uint8_t, 4, 0>::decode(0b00001001) == 0b1001);
    REQUIRE(BitCodec<std::uint8_t, 4, 1>::decode(0b00010010) == 0b1001);
    REQUIRE(BitCodec<std::uint8_t, 4, 2>::decode(0b00100100) == 0b1001);
    REQUIRE(BitCodec<std::uint8_t, 4, 3>::decode(0b01001000) == 0b1001);
    REQUIRE(BitCodec<std::uint8_t, 4, 4>::decode(0b10010000) == 0b1001);
}

TEST_CASE("BitCodec encodes into the offset bits", "[machine][bitcodec]")
{
    REQUIRE(BitCodec<std::uint8_t, 4, 0>::encode(0b1001) == 0b00001001);
    REQUIRE(BitCodec<std::uint8_t, 4, 1>::encode(0b1001) == 0b00010010);
    REQUIRE(BitCodec<std::uint8_t, 4, 2>::encode(0b1001) == 0b00100100);
    REQUIRE(BitCodec<std::uint8_t, 4, 3>::encode(0b1001) == 0b01001000);
    REQUIRE(BitCodec<std::uint8_t, 4, 4>::encode(0b1001) == 0b10010000);
}
