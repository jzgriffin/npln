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
#include <optional>

namespace libnpln::machine {

using Memory = std::array<Byte, 0x1000>;

constexpr Address program_address = 0x200;
constexpr std::size_t memory_size = Memory{}.size();

auto load_into_memory(std::istream& s, Memory& m, Address const a)
    -> bool;
auto load_into_memory(std::filesystem::path const& p, Memory& m,
    Address const a) -> bool;
auto load_program(std::istream& s) -> std::optional<Memory>;
auto load_program(std::filesystem::path const& p)
    -> std::optional<Memory>;

}

#endif
