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

#include <libnpln/machine/Fault.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("Fault types define names", "[machine][fault]")
{
    REQUIRE(get_name(Fault::Type::invalid_address) == "invalid_address");
    REQUIRE(get_name(Fault::Type::invalid_instruction) == "invalid_instruction");
    REQUIRE(get_name(Fault::Type::invalid_digit) == "invalid_digit");
    REQUIRE(get_name(Fault::Type::empty_stack) == "empty_stack");
    REQUIRE(get_name(Fault::Type::full_stack) == "full_stack");
}

TEST_CASE("Fault type returns its name when formatted",
    "[machine][fault]")
{
    REQUIRE(fmt::format("{}", Fault::Type::invalid_address) == get_name(Fault::Type::invalid_address));
    REQUIRE(fmt::format("{}", Fault::Type::invalid_instruction) == get_name(Fault::Type::invalid_instruction));
    REQUIRE(fmt::format("{}", Fault::Type::invalid_digit) == get_name(Fault::Type::invalid_digit));
    REQUIRE(fmt::format("{}", Fault::Type::empty_stack) == get_name(Fault::Type::empty_stack));
    REQUIRE(fmt::format("{}", Fault::Type::full_stack) == get_name(Fault::Type::full_stack));
}

TEST_CASE("Fault formats correctly", "[machine][fault]")
{
    REQUIRE(fmt::format("{}", Fault{Fault::Type::invalid_address, 0x123}) ==
        "invalid_address@123h");
    REQUIRE(fmt::format("{}", Fault{Fault::Type::invalid_instruction, 0x456}) ==
        "invalid_instruction@456h");
    REQUIRE(fmt::format("{}", Fault{Fault::Type::invalid_digit, 0x768}) ==
        "invalid_digit@768h");
    REQUIRE(fmt::format("{}", Fault{Fault::Type::empty_stack, 0xABC}) ==
        "empty_stack@ABCh");
    REQUIRE(fmt::format("{}", Fault{Fault::Type::full_stack, 0xDEF}) ==
        "full_stack@DEFh");
}
