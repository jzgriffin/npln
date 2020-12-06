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

#ifndef LIBNPLN_MACHINE_MEMORY_HPP
#define LIBNPLN_MACHINE_MEMORY_HPP

#include <libnpln/machine/DataUnits.hpp>

#include <array>
#include <filesystem>
#include <iosfwd>
#include <iterator>

namespace libnpln::machine {

using Memory = std::array<Byte, 0x1000>;

constexpr std::size_t memory_size = Memory{}.size();

template<typename InputIter>
auto load_into_memory(
    InputIter first, InputIter last, Memory& m, Address const a) -> bool
{
    if (m.size() < a) {
        return false;
    }

    auto ii = first;
    auto oi = std::next(std::begin(m), a);
    while (ii != last && oi != std::end(m)) {
        *oi++ = *ii++;
    }

    return ii == last;
}

auto load_into_memory(std::istream& s, Memory& m, Address const a) -> bool;
auto load_into_memory(
    std::filesystem::path const& p, Memory& m, Address const a) -> bool;

template<Address A, std::size_t N>
    auto load_into_memory(Byte const (&b)[N], Memory& m)
        -> std::enable_if_t < A + N<memory_size, void>
{
    load_into_memory(std::begin(b), std::end(b), m, A);
}

} // namespace libnpln::machine

#endif
