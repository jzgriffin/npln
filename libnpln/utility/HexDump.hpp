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

#ifndef LIBNPLN_UTILITY_HEXDUMP_HPP
#define LIBNPLN_UTILITY_HEXDUMP_HPP

#include <fmt/format.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <string>

namespace libnpln::utility {

template<std::size_t N>
auto to_hex_dump(std::array<std::uint8_t, N> const& a) -> std::string
{
    const auto index_nibbles =
        static_cast<std::size_t>(std::ceil(std::log(a.size()) / std::log(16)));

    std::string str;
    auto out = std::back_inserter(str);
    auto zero_rows = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (i % 0x10 == 0) {
            auto zero = true;
            for (auto j = i; j < a.size() && j < i + 0x10; ++j) {
                if (a[j] != 0x00) {
                    zero = false;
                    break;
                }
            }

            if (zero) {
                ++zero_rows;
            }
            else {
                zero_rows = 0;
            }

            if (i > 0 && zero_rows <= 2) {
                out = fmt::format_to(out, "\n");
            }

            if (zero_rows <= 1) {
                out = fmt::format_to(out, "{:0{}X}:", i, index_nibbles);
            }
            else if (zero_rows == 2) {
                out = fmt::format_to(out, "{:.{}}:", "", index_nibbles);
            }
        }

        if (zero_rows <= 1) {
            out = fmt::format_to(out, " {:02X}", a[i]);
        }
        else if (zero_rows == 2) {
            out = fmt::format_to(out, " ..");
        }
    }
    return str;
}

} // namespace libnpln::utility

#endif
