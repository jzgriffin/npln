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

#ifndef LIBNPLN_MACHINE_KEY_HPP
#define LIBNPLN_MACHINE_KEY_HPP

#include <libnpln/machine/DataUnits.hpp>

#include <fmt/format.h>

#include <stdexcept>
#include <string_view>

namespace libnpln::machine {

enum class Key : Byte
{
    k0 = 0x0,
    k1 = 0x1,
    k2 = 0x2,
    k3 = 0x3,
    k4 = 0x4,
    k5 = 0x5,
    k6 = 0x6,
    k7 = 0x7,
    k8 = 0x8,
    k9 = 0x9,
    ka = 0xA,
    kb = 0xB,
    kc = 0xC,
    kd = 0xD,
    ke = 0xE,
    kf = 0xF,
};

constexpr std::size_t key_count = 16;

constexpr auto to_index(Key const k) noexcept -> std::size_t
{
    return static_cast<std::size_t>(k);
}

constexpr auto get_name(Key const k) -> std::string_view
{
    switch (k) {
    case Key::k0: return "0";
    case Key::k1: return "1";
    case Key::k2: return "2";
    case Key::k3: return "3";
    case Key::k4: return "4";
    case Key::k5: return "5";
    case Key::k6: return "6";
    case Key::k7: return "7";
    case Key::k8: return "8";
    case Key::k9: return "9";
    case Key::ka: return "A";
    case Key::kb: return "B";
    case Key::kc: return "C";
    case Key::kd: return "D";
    case Key::ke: return "E";
    case Key::kf: return "F";
    }

    throw std::out_of_range("Unknown Key in get_name");
}

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Key>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Key const& value, FormatContext& context)
    {
        return format_to(context.out(), "{}", libnpln::machine::get_name(value));
    }
};

#endif
