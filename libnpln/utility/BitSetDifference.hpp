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

#ifndef LIBNPLN_UTILITY_BITSETDIFFERENCE_HPP
#define LIBNPLN_UTILITY_BITSETDIFFERENCE_HPP

#include <bitset>
#include <set>

namespace libnpln::utility {

template<std::size_t TBitCount>
auto bit_set_difference(std::bitset<TBitCount> const& initial,
    std::bitset<TBitCount> const& final) -> std::set<std::size_t>
{
    auto const diff = initial ^ final;
    std::set<std::size_t> positions;
    for (std::size_t i = 0; i < TBitCount; ++i) {
        if (diff.test(i)) {
            positions.insert(i);
        }
    }

    return positions;
}

// Returns the difference between the initial and final bit sets.  The first
// set returned is the bit indices that became set and the second set is the
// bit indices that became reset.
template<std::size_t TBitCount>
auto bit_set_split_difference(std::bitset<TBitCount> const& initial,
    std::bitset<TBitCount> const& final) noexcept ->
    std::pair<std::set<std::size_t>, std::set<std::size_t>>
{
    auto sbs = std::set<std::size_t>{};
    auto rbs = std::set<std::size_t>{};
    for (auto&& i : bit_set_difference(initial, final)) {
        (final.test(i) ? sbs : rbs).insert(i);
    }

    return {sbs, rbs};
}

}

#endif
