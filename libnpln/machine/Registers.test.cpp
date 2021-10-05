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

#include <libnpln/machine/Registers.hpp>

#include <catch2/catch.hpp>

#include <limits>
#include <stdexcept>
#include <type_traits>

using namespace libnpln::machine;

TEST_CASE("Registers are of acceptable widths", "[machine][registers]")
{
    SECTION("General-purpose registers are 8 bits wide")
    {
        auto const rs = Registers{};
        REQUIRE(std::numeric_limits<decltype(rs.v0)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v1)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v2)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v3)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v4)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v5)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v6)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v7)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v8)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.v9)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.va)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.vb)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.vc)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.vd)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.ve)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.vf)>::digits == 8);
    }

    SECTION("Timer registers are 8 bits wide")
    {
        auto const rs = Registers{};
        REQUIRE(std::numeric_limits<decltype(rs.dt)>::digits == 8);
        REQUIRE(std::numeric_limits<decltype(rs.st)>::digits == 8);
    }

    SECTION("The index register is 16 bits wide")
    {
        auto const rs = Registers{};
        REQUIRE(std::numeric_limits<decltype(rs.i)>::digits == 16);
    }
}

TEST_CASE("Registers can be indexed by general-purpose registers", "[machine][registers]")
{
    SECTION("constant operator[] returns references")
    {
        auto const rs = Registers{};
        REQUIRE(&rs[Register::v0] == &rs.v0);
        REQUIRE(&rs[Register::v1] == &rs.v1);
        REQUIRE(&rs[Register::v2] == &rs.v2);
        REQUIRE(&rs[Register::v3] == &rs.v3);
        REQUIRE(&rs[Register::v4] == &rs.v4);
        REQUIRE(&rs[Register::v5] == &rs.v5);
        REQUIRE(&rs[Register::v6] == &rs.v6);
        REQUIRE(&rs[Register::v7] == &rs.v7);
        REQUIRE(&rs[Register::v8] == &rs.v8);
        REQUIRE(&rs[Register::v9] == &rs.v9);
        REQUIRE(&rs[Register::va] == &rs.va);
        REQUIRE(&rs[Register::vb] == &rs.vb);
        REQUIRE(&rs[Register::vc] == &rs.vc);
        REQUIRE(&rs[Register::vd] == &rs.vd);
        REQUIRE(&rs[Register::ve] == &rs.ve);
        REQUIRE(&rs[Register::vf] == &rs.vf);
    }

    SECTION("mutable operator[] returns references")
    {
        auto rs = Registers{};
        REQUIRE(&rs[Register::v0] == &rs.v0);
        REQUIRE(&rs[Register::v1] == &rs.v1);
        REQUIRE(&rs[Register::v2] == &rs.v2);
        REQUIRE(&rs[Register::v3] == &rs.v3);
        REQUIRE(&rs[Register::v4] == &rs.v4);
        REQUIRE(&rs[Register::v5] == &rs.v5);
        REQUIRE(&rs[Register::v6] == &rs.v6);
        REQUIRE(&rs[Register::v7] == &rs.v7);
        REQUIRE(&rs[Register::v8] == &rs.v8);
        REQUIRE(&rs[Register::v9] == &rs.v9);
        REQUIRE(&rs[Register::va] == &rs.va);
        REQUIRE(&rs[Register::vb] == &rs.vb);
        REQUIRE(&rs[Register::vc] == &rs.vc);
        REQUIRE(&rs[Register::vd] == &rs.vd);
        REQUIRE(&rs[Register::ve] == &rs.ve);
        REQUIRE(&rs[Register::vf] == &rs.vf);
    }
}

TEST_CASE(
    "Registers cannot be indexed by unknown general-purpose registers", "[machine][registers]")
{
    auto const invalid_register =
        static_cast<Register>(std::numeric_limits<std::underlying_type_t<Register>>::max());

    auto const rs_const = Registers{};
    REQUIRE_THROWS_AS(rs_const[invalid_register], std::out_of_range);

    auto rs = Registers{};
    REQUIRE_THROWS_AS(rs[invalid_register], std::out_of_range);
}

TEST_CASE("Indexed general-purpose register writes can be read back", "[machine][registers]")
{
    auto rs = Registers{};
    rs[Register::v0] = 0xCC;
    rs[Register::v1] = 0xBB;
    rs[Register::v2] = 0xAA;
    rs[Register::v3] = 0x76;
    rs[Register::v4] = 0xBA;
    rs[Register::v5] = 0x98;
    rs[Register::v6] = 0xDC;
    rs[Register::v7] = 0x89;
    rs[Register::v8] = 0xFE;
    rs[Register::v9] = 0x67;
    rs[Register::va] = 0xEF;
    rs[Register::vb] = 0x45;
    rs[Register::vc] = 0xCD;
    rs[Register::vd] = 0x23;
    rs[Register::ve] = 0xAB;
    rs[Register::vf] = 0x01;
    REQUIRE(rs[Register::v0] == 0xCC);
    REQUIRE(rs[Register::v1] == 0xBB);
    REQUIRE(rs[Register::v2] == 0xAA);
    REQUIRE(rs[Register::v3] == 0x76);
    REQUIRE(rs[Register::v4] == 0xBA);
    REQUIRE(rs[Register::v5] == 0x98);
    REQUIRE(rs[Register::v6] == 0xDC);
    REQUIRE(rs[Register::v7] == 0x89);
    REQUIRE(rs[Register::v8] == 0xFE);
    REQUIRE(rs[Register::v9] == 0x67);
    REQUIRE(rs[Register::va] == 0xEF);
    REQUIRE(rs[Register::vb] == 0x45);
    REQUIRE(rs[Register::vc] == 0xCD);
    REQUIRE(rs[Register::vd] == 0x23);
    REQUIRE(rs[Register::ve] == 0xAB);
    REQUIRE(rs[Register::vf] == 0x01);
}
