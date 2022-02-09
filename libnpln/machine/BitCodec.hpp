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

#ifndef LIBNPLN_MACHINE_BITCODEC_HPP
#define LIBNPLN_MACHINE_BITCODEC_HPP

#include <cstddef>
#include <limits>

namespace libnpln::machine {

template<typename TContainer, std::size_t TBitWidth, std::size_t TBitOffset>
struct BitCodec
{
    using Container = TContainer;

    static constexpr auto decode(Container const x) noexcept
    {
        return static_cast<Container>((x & bit_mask) >> bit_offset);
    }

    static constexpr auto encode(Container const x) noexcept
    {
        return static_cast<Container>((x << bit_offset) & bit_mask);
    }

    static constexpr auto bit_width = TBitWidth;
    static constexpr auto bit_offset = TBitOffset;
    static_assert(bit_width + bit_offset <= std::numeric_limits<Container>::digits);

    static constexpr auto bit_mask =
        static_cast<Container>((std::numeric_limits<Container>::max()
                                   >> (std::numeric_limits<Container>::digits - bit_width))
            << bit_offset);
};

} // namespace libnpln::machine

#endif
