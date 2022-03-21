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

#ifndef LIBNPLN_DISASSEMBLER_TABLE_HPP
#define LIBNPLN_DISASSEMBLER_TABLE_HPP

#include <libnpln/disassembler/Row.hpp>
#include <libnpln/machine/DataUnits.hpp>

#include <string_view>
#include <vector>

namespace libnpln::disassembler {

using Table = std::vector<Row>;

auto find_address(Table const& table, machine::Address addr) -> Table::const_iterator;
auto find_address(Table& table, machine::Address addr) -> Table::iterator;

auto find_label(Table const& table, std::string_view label) -> Table::const_iterator;
auto find_label(Table& table, std::string_view label) -> Table::iterator;

auto insert_row(Table& table, Row const& row) -> Table::iterator;
auto insert_row(Table& table, Row&& row) -> Table::iterator;

} // namespace libnpln::disassembler

#endif
