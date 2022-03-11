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

#ifndef LIBNPLN_MACHINE_STACK_HPP
#define LIBNPLN_MACHINE_STACK_HPP

#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/utility/FixedSizeStack.hpp>

#include <fmt/format.h>

#include <iterator>

namespace libnpln::machine {

using Stack = utility::FixedSizeStack<Address, 16>;

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Stack>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Stack const& value, FormatContext& context)
    {
        auto out = context.out();
        for (auto i = std::begin(value); i != std::end(value); ++i) {
            if (i != std::begin(value)) {
                out = format_to(out, ", ");
            }
            out = format_to(out, "{:03X}h", *i);
        }
        return out;
    }
};

#endif
