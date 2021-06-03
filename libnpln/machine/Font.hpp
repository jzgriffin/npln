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

#ifndef LIBNPLN_MACHINE_FONT_HPP
#define LIBNPLN_MACHINE_FONT_HPP

#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Memory.hpp>

#include <array>
#include <optional>

namespace libnpln::machine {

// Each font glyph is 8x5 pixels
using Glyph = std::array<Byte, 5>;

constexpr std::size_t glyph_size = Glyph{}.size();

constexpr std::array<Glyph, 0x10> font_glyphs = {
    Glyph{
        // 0
        0b1111'0000,
        0b1001'0000,
        0b1001'0000,
        0b1001'0000,
        0b1111'0000,
    },
    Glyph{
        // 1
        0b0010'0000,
        0b0110'0000,
        0b0010'0000,
        0b0010'0000,
        0b0111'0000,
    },
    Glyph{
        // 2
        0b1111'0000,
        0b0001'0000,
        0b1111'0000,
        0b1000'0000,
        0b1111'0000,
    },
    Glyph{
        // 3
        0b1111'0000,
        0b0001'0000,
        0b1111'0000,
        0b0001'0000,
        0b1111'0000,
    },
    Glyph{
        // 4
        0b1001'0000,
        0b1001'0000,
        0b1111'0000,
        0b0001'0000,
        0b0001'0000,
    },
    Glyph{
        // 5
        0b1111'0000,
        0b1000'0000,
        0b1111'0000,
        0b0001'0000,
        0b1111'0000,
    },
    Glyph{
        // 6
        0b1111'0000,
        0b1000'0000,
        0b1111'0000,
        0b1001'0000,
        0b1111'0000,
    },
    Glyph{
        // 7
        0b1111'0000,
        0b0001'0000,
        0b0010'0000,
        0b0100'0000,
        0b0100'0000,
    },
    Glyph{
        // 8
        0b1111'0000,
        0b1001'0000,
        0b1111'0000,
        0b1001'0000,
        0b1111'0000,
    },
    Glyph{
        // 9
        0b1111'0000,
        0b1001'0000,
        0b1111'0000,
        0b0001'0000,
        0b1111'0000,
    },
    Glyph{
        // A
        0b1111'0000,
        0b1001'0000,
        0b1111'0000,
        0b1001'0000,
        0b1001'0000,
    },
    Glyph{
        // B
        0b1110'0000,
        0b1001'0000,
        0b1110'0000,
        0b1001'0000,
        0b1110'0000,
    },
    Glyph{
        // C
        0b1111'0000,
        0b1000'0000,
        0b1000'0000,
        0b1000'0000,
        0b1111'0000,
    },
    Glyph{
        // D
        0b1110'0000,
        0b1001'0000,
        0b1001'0000,
        0b1001'0000,
        0b1110'0000,
    },
    Glyph{
        // E
        0b1111'0000,
        0b1000'0000,
        0b1111'0000,
        0b1000'0000,
        0b1111'0000,
    },
    Glyph{
        // F
        0b1111'0000,
        0b1000'0000,
        0b1111'0000,
        0b1000'0000,
        0b1000'0000,
    },
};

auto load_font_into_memory(Memory& m, Address const a) -> bool;

constexpr auto get_glyph_offset(Nibble const digit) -> std::optional<std::size_t>
{
    if (digit >= font_glyphs.size()) {
        return std::nullopt;
    }

    return glyph_size * digit;
}

} // namespace libnpln::machine

#endif
