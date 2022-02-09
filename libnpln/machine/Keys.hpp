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

#ifndef LIBNPLN_MACHINE_KEYS_HPP
#define LIBNPLN_MACHINE_KEYS_HPP

#include <libnpln/machine/Key.hpp>

#include <fmt/format.h>

#include <bitset>
#include <cstdint>
#include <set>

namespace libnpln::machine {

using Keys = std::bitset<key_count>;

auto keys_difference(Keys const& initial, Keys const& final) noexcept
    -> std::pair<std::set<Key>, std::set<Key>>;

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Keys>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Keys const& value, FormatContext& context)
    {
        auto out = context.out();
        for (std::size_t i = 0; i < value.size(); ++i) {
            if (value.test(i)) {
                out = format_to(out, "{}", static_cast<libnpln::machine::Key>(i));
            }
        }
        return out;
    }
};

#endif
