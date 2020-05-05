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
#include <optional>

namespace libnpln::machine {

using Memory = std::array<Byte, 0x1000>;

constexpr Address program_address = 0x200;
constexpr std::size_t memory_size = Memory{}.size();

template<typename InputIter>
auto load_into_memory(InputIter first, InputIter last, Memory& m,
    Address const a) -> bool
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

auto load_into_memory(std::istream& s, Memory& m, Address const a)
    -> bool;
auto load_into_memory(std::filesystem::path const& p, Memory& m,
    Address const a) -> bool;

template<typename InputIter>
auto load_program(InputIter first, InputIter last) -> std::optional<Memory>
{
    auto m = Memory{};
    return load_into_memory(first, last, m, program_address)
        ? std::optional{m} : std::nullopt;
}

auto load_program(std::istream& s) -> std::optional<Memory>;
auto load_program(std::filesystem::path const& p)
    -> std::optional<Memory>;

template<std::size_t N>
auto create_program(Byte const (&p)[N]) ->
    std::enable_if_t<program_address + N < memory_size, Memory>
{
    return *load_program(std::begin(p), std::end(p));
}

}

#endif
