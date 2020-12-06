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

#include <libnpln/machine/Font.hpp>

#include <catch2/catch.hpp>
#include <gsl/gsl>

#include <iterator>
#include <map>
#include <type_traits>

using namespace libnpln::machine;

TEST_CASE("Glyphs store 5 bytes", "[machine][font]")
{
    static_assert(std::is_same_v<Glyph::value_type, Byte>);
    REQUIRE(Glyph{}.max_size() == 5);
    REQUIRE(Glyph{}.max_size() == glyph_size);
}

TEST_CASE("Font can be loaded into memory", "[machine][font]")
{
    Memory m;
    constexpr Address base_addr = 0x100;
    REQUIRE(load_font_into_memory(m, base_addr));

    auto* const base_iter = std::next(std::begin(m), base_addr);
    for (std::size_t i = 0; i < font_glyphs.size(); ++i) {
        auto const& g = gsl::at(font_glyphs, i);
        REQUIRE(std::equal(
            std::next(base_iter, g.size() * i),
            std::next(base_iter, g.size() * (i + 1)),
            std::begin(g), std::end(g)));
    }
}

TEST_CASE("Font cannot be loaded past the end of memory", "[machine][font]")
{
    Memory m;
    constexpr Address a = m.size() - glyph_size * font_glyphs.size() + 1;
    REQUIRE_FALSE(load_font_into_memory(m, a));
}

TEST_CASE("Glyph offsets can be calculated", "[machine][font]")
{
    std::map<Nibble, std::size_t> const offsets = {
        {0x0, 0},
        {0x1, 5},
        {0x2, 10},
        {0x3, 15},
        {0x4, 20},
        {0x5, 25},
        {0x6, 30},
        {0x7, 35},
        {0x8, 40},
        {0x9, 45},
        {0xA, 50},
        {0xB, 55},
        {0xC, 60},
        {0xD, 65},
        {0xE, 70},
        {0xF, 75},
    };
    for (auto&& [g, o] : offsets) {
        REQUIRE(get_glyph_offset(g) == o);
    }
}

TEST_CASE("Unknown glyph offsets cannot be calculated", "[machine][font]")
{
    REQUIRE(get_glyph_offset(0x10) == std::nullopt);
}
