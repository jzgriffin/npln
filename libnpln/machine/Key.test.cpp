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
#include <libnpln/machine/Key.hpp>

#include <catch2/catch.hpp>

using namespace libnpln;
using namespace libnpln::machine;

TEST_CASE("Key meets CHIP-8 specifications", "[machine][key]")
{
    REQUIRE(detail::to_underlying(Key::k0) == 0x0);
    REQUIRE(detail::to_underlying(Key::k1) == 0x1);
    REQUIRE(detail::to_underlying(Key::k2) == 0x2);
    REQUIRE(detail::to_underlying(Key::k3) == 0x3);
    REQUIRE(detail::to_underlying(Key::k4) == 0x4);
    REQUIRE(detail::to_underlying(Key::k5) == 0x5);
    REQUIRE(detail::to_underlying(Key::k6) == 0x6);
    REQUIRE(detail::to_underlying(Key::k7) == 0x7);
    REQUIRE(detail::to_underlying(Key::k8) == 0x8);
    REQUIRE(detail::to_underlying(Key::k9) == 0x9);
    REQUIRE(detail::to_underlying(Key::ka) == 0xA);
    REQUIRE(detail::to_underlying(Key::kb) == 0xB);
    REQUIRE(detail::to_underlying(Key::kc) == 0xC);
    REQUIRE(detail::to_underlying(Key::kd) == 0xD);
    REQUIRE(detail::to_underlying(Key::ke) == 0xE);
    REQUIRE(detail::to_underlying(Key::kf) == 0xF);
}

TEST_CASE("Key can be converted to a bit index", "[machine][key]")
{
    REQUIRE(to_index(Key::k0) == detail::to_underlying(Key::k0));
    REQUIRE(to_index(Key::k1) == detail::to_underlying(Key::k1));
    REQUIRE(to_index(Key::k2) == detail::to_underlying(Key::k2));
    REQUIRE(to_index(Key::k3) == detail::to_underlying(Key::k3));
    REQUIRE(to_index(Key::k4) == detail::to_underlying(Key::k4));
    REQUIRE(to_index(Key::k5) == detail::to_underlying(Key::k5));
    REQUIRE(to_index(Key::k6) == detail::to_underlying(Key::k6));
    REQUIRE(to_index(Key::k7) == detail::to_underlying(Key::k7));
    REQUIRE(to_index(Key::k8) == detail::to_underlying(Key::k8));
    REQUIRE(to_index(Key::k9) == detail::to_underlying(Key::k9));
    REQUIRE(to_index(Key::ka) == detail::to_underlying(Key::ka));
    REQUIRE(to_index(Key::kb) == detail::to_underlying(Key::kb));
    REQUIRE(to_index(Key::kc) == detail::to_underlying(Key::kc));
    REQUIRE(to_index(Key::kd) == detail::to_underlying(Key::kd));
    REQUIRE(to_index(Key::ke) == detail::to_underlying(Key::ke));
    REQUIRE(to_index(Key::kf) == detail::to_underlying(Key::kf));
}

TEST_CASE("Key defines names", "[machine][key]")
{
    REQUIRE(get_name(Key::k0) == "0");
    REQUIRE(get_name(Key::k1) == "1");
    REQUIRE(get_name(Key::k2) == "2");
    REQUIRE(get_name(Key::k3) == "3");
    REQUIRE(get_name(Key::k4) == "4");
    REQUIRE(get_name(Key::k5) == "5");
    REQUIRE(get_name(Key::k6) == "6");
    REQUIRE(get_name(Key::k7) == "7");
    REQUIRE(get_name(Key::k8) == "8");
    REQUIRE(get_name(Key::k9) == "9");
    REQUIRE(get_name(Key::ka) == "A");
    REQUIRE(get_name(Key::kb) == "B");
    REQUIRE(get_name(Key::kc) == "C");
    REQUIRE(get_name(Key::kd) == "D");
    REQUIRE(get_name(Key::ke) == "E");
    REQUIRE(get_name(Key::kf) == "F");
}

TEST_CASE("Key returns its name when formatted", "[machine][key]")
{
    REQUIRE(fmt::format("{}", Key::k0) == get_name(Key::k0));
    REQUIRE(fmt::format("{}", Key::k1) == get_name(Key::k1));
    REQUIRE(fmt::format("{}", Key::k2) == get_name(Key::k2));
    REQUIRE(fmt::format("{}", Key::k3) == get_name(Key::k3));
    REQUIRE(fmt::format("{}", Key::k4) == get_name(Key::k4));
    REQUIRE(fmt::format("{}", Key::k5) == get_name(Key::k5));
    REQUIRE(fmt::format("{}", Key::k6) == get_name(Key::k6));
    REQUIRE(fmt::format("{}", Key::k7) == get_name(Key::k7));
    REQUIRE(fmt::format("{}", Key::k8) == get_name(Key::k8));
    REQUIRE(fmt::format("{}", Key::k9) == get_name(Key::k9));
    REQUIRE(fmt::format("{}", Key::ka) == get_name(Key::ka));
    REQUIRE(fmt::format("{}", Key::kb) == get_name(Key::kb));
    REQUIRE(fmt::format("{}", Key::kc) == get_name(Key::kc));
    REQUIRE(fmt::format("{}", Key::kd) == get_name(Key::kd));
    REQUIRE(fmt::format("{}", Key::ke) == get_name(Key::ke));
    REQUIRE(fmt::format("{}", Key::kf) == get_name(Key::kf));
}
