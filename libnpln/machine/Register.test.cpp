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

#include <libnpln/detail/cpp2b.hpp>
#include <libnpln/machine/Register.hpp>

#include <catch2/catch.hpp>

using namespace libnpln;
using namespace libnpln::machine;

TEST_CASE("Registers meet CHIP-8 specifications", "[machine][register]")
{
    REQUIRE(detail::to_underlying(Register::v0) == 0x0);
    REQUIRE(detail::to_underlying(Register::v1) == 0x1);
    REQUIRE(detail::to_underlying(Register::v2) == 0x2);
    REQUIRE(detail::to_underlying(Register::v3) == 0x3);
    REQUIRE(detail::to_underlying(Register::v4) == 0x4);
    REQUIRE(detail::to_underlying(Register::v5) == 0x5);
    REQUIRE(detail::to_underlying(Register::v6) == 0x6);
    REQUIRE(detail::to_underlying(Register::v7) == 0x7);
    REQUIRE(detail::to_underlying(Register::v8) == 0x8);
    REQUIRE(detail::to_underlying(Register::v9) == 0x9);
    REQUIRE(detail::to_underlying(Register::va) == 0xA);
    REQUIRE(detail::to_underlying(Register::vb) == 0xB);
    REQUIRE(detail::to_underlying(Register::vc) == 0xC);
    REQUIRE(detail::to_underlying(Register::vd) == 0xD);
    REQUIRE(detail::to_underlying(Register::ve) == 0xE);
    REQUIRE(detail::to_underlying(Register::vf) == 0xF);
}

TEST_CASE("Registers define names", "[machine][register]")
{
    REQUIRE(get_name(Register::v0) == "V0");
    REQUIRE(get_name(Register::v1) == "V1");
    REQUIRE(get_name(Register::v2) == "V2");
    REQUIRE(get_name(Register::v3) == "V3");
    REQUIRE(get_name(Register::v4) == "V4");
    REQUIRE(get_name(Register::v5) == "V5");
    REQUIRE(get_name(Register::v6) == "V6");
    REQUIRE(get_name(Register::v7) == "V7");
    REQUIRE(get_name(Register::v8) == "V8");
    REQUIRE(get_name(Register::v9) == "V9");
    REQUIRE(get_name(Register::va) == "VA");
    REQUIRE(get_name(Register::vb) == "VB");
    REQUIRE(get_name(Register::vc) == "VC");
    REQUIRE(get_name(Register::vd) == "VD");
    REQUIRE(get_name(Register::ve) == "VE");
    REQUIRE(get_name(Register::vf) == "VF");
}

TEST_CASE("Register returns its name when formatted", "[machine][register]")
{
    REQUIRE(fmt::format("{}", Register::v0) == get_name(Register::v0));
    REQUIRE(fmt::format("{}", Register::v1) == get_name(Register::v1));
    REQUIRE(fmt::format("{}", Register::v2) == get_name(Register::v2));
    REQUIRE(fmt::format("{}", Register::v3) == get_name(Register::v3));
    REQUIRE(fmt::format("{}", Register::v4) == get_name(Register::v4));
    REQUIRE(fmt::format("{}", Register::v5) == get_name(Register::v5));
    REQUIRE(fmt::format("{}", Register::v6) == get_name(Register::v6));
    REQUIRE(fmt::format("{}", Register::v7) == get_name(Register::v7));
    REQUIRE(fmt::format("{}", Register::v8) == get_name(Register::v8));
    REQUIRE(fmt::format("{}", Register::v9) == get_name(Register::v9));
    REQUIRE(fmt::format("{}", Register::va) == get_name(Register::va));
    REQUIRE(fmt::format("{}", Register::vb) == get_name(Register::vb));
    REQUIRE(fmt::format("{}", Register::vc) == get_name(Register::vc));
    REQUIRE(fmt::format("{}", Register::vd) == get_name(Register::vd));
    REQUIRE(fmt::format("{}", Register::ve) == get_name(Register::ve));
    REQUIRE(fmt::format("{}", Register::vf) == get_name(Register::vf));
}
