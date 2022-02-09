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

#include <libnpln/utility/Numeric.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::utility;

TEST_CASE("Unsigned addition overflow is detected", "[utility][numeric]")
{
    REQUIRE(addition_overflow<std::uint8_t>(0xFF, 0xFF));
    REQUIRE(addition_overflow<std::uint8_t>(0xFF, 0x01));
    REQUIRE(addition_overflow<std::uint8_t>(0x01, 0xFF));
    REQUIRE_FALSE(addition_overflow<std::uint8_t>(0x00, 0xFF));
    REQUIRE_FALSE(addition_overflow<std::uint8_t>(0x00, 0x00));
    REQUIRE_FALSE(addition_overflow<std::uint8_t>(0xFF, 0x00));
    REQUIRE_FALSE(addition_overflow<std::uint8_t>(0xFE, 0x01));
}

TEST_CASE("Unsigned subtraction underflow is detected", "[utility][numeric]")
{
    REQUIRE_FALSE(subtraction_underflow<std::uint8_t>(0xFF, 0xFF));
    REQUIRE_FALSE(subtraction_underflow<std::uint8_t>(0xFF, 0x01));
    REQUIRE(subtraction_underflow<std::uint8_t>(0x01, 0xFF));
    REQUIRE(subtraction_underflow<std::uint8_t>(0x00, 0xFF));
    REQUIRE_FALSE(subtraction_underflow<std::uint8_t>(0x00, 0x00));
    REQUIRE_FALSE(subtraction_underflow<std::uint8_t>(0xFF, 0x00));
    REQUIRE_FALSE(subtraction_underflow<std::uint8_t>(0xFE, 0x01));
}

TEST_CASE("Least significant bit is checked", "[utility][numeric]")
{
    REQUIRE(lsb<std::uint8_t>(0b11111111));
    REQUIRE(lsb<std::uint8_t>(0b00000001));
    REQUIRE(lsb<std::uint8_t>(0b10011001));
    REQUIRE_FALSE(lsb<std::uint8_t>(0b00000000));
    REQUIRE_FALSE(lsb<std::uint8_t>(0b11111110));
    REQUIRE_FALSE(lsb<std::uint8_t>(0b10101010));
    REQUIRE_FALSE(lsb<std::uint8_t>(0b01010100));
}

TEST_CASE("Most significant bit is checked", "[utility][numeric]")
{
    REQUIRE(msb<std::uint8_t>(0b11111111));
    REQUIRE(msb<std::uint8_t>(0b10000000));
    REQUIRE(msb<std::uint8_t>(0b10011001));
    REQUIRE_FALSE(msb<std::uint8_t>(0b00000000));
    REQUIRE_FALSE(msb<std::uint8_t>(0b01111111));
    REQUIRE_FALSE(msb<std::uint8_t>(0b00101010));
    REQUIRE_FALSE(msb<std::uint8_t>(0b01010101));
}
