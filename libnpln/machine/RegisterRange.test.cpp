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

#include <libnpln/machine/RegisterRange.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("Register iterator starts at the correct register",
    "[machine][register][iterator]")
{
    SECTION("from %V0")
    {
        auto current = RegisterIterator{Register::v0};
        auto const last = RegisterIterator{};
        REQUIRE(current != last);
        REQUIRE(*current == Register::v0);
    }

    SECTION("from %VA")
    {
        auto current = RegisterIterator{Register::va};
        auto const last = RegisterIterator{};
        REQUIRE(current != last);
        REQUIRE(*current == Register::va);
    }

    SECTION("from %VF")
    {
        auto current = RegisterIterator{Register::vf};
        auto const last = RegisterIterator{};
        REQUIRE(current != last);
        REQUIRE(*current == Register::vf);
    }
}

TEST_CASE("Register iterator advances to the next register",
    "[machine][register][iterator]")
{
    SECTION("from %V0")
    {
        auto current = RegisterIterator{Register::v0};
        ++current;
        auto const last = RegisterIterator{};
        REQUIRE(current != last);
        REQUIRE(*current == Register::v1);
    }

    SECTION("from %VA")
    {
        auto current = RegisterIterator{Register::va};
        ++current;
        auto const last = RegisterIterator{};
        REQUIRE(current != last);
        REQUIRE(*current == Register::vb);
    }

    SECTION("from %VF")
    {
        auto current = RegisterIterator{Register::vf};
        ++current;
        auto const last = RegisterIterator{};
        REQUIRE(current == last);
    }
}

TEST_CASE("Register range produces accurate iterators")
{
    SECTION("for the complete set of registers")
    {
        auto const r = RegisterRange{};
        auto const first = RegisterIterator{Register::v0};
        auto const last = RegisterIterator{};
        REQUIRE(std::begin(r) == first);
        REQUIRE(std::end(r) == last);
    }

    SECTION("for a prefix of the sequence of registers")
    {
        auto const r = RegisterRange{Register::v4};
        auto const first = RegisterIterator{Register::v0};
        auto const last = RegisterIterator{Register::v5};
        REQUIRE(std::begin(r) == first);
        REQUIRE(std::end(r) == last);
    }

    SECTION("for a suffix of the sequence of registers")
    {
        auto const r = RegisterRange{Register::v4, Register::vf};
        auto const first = RegisterIterator{Register::v4};
        auto const last = RegisterIterator{};
        REQUIRE(std::begin(r) == first);
        REQUIRE(std::end(r) == last);
    }

    SECTION("for an inner subsequence of the sequence of registers")
    {
        auto const r = RegisterRange{Register::v4, Register::va};
        auto const first = RegisterIterator{Register::v4};
        auto const last = RegisterIterator{Register::vb};
        REQUIRE(std::begin(r) == first);
        REQUIRE(std::end(r) == last);
    }
}
