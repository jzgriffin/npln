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

#include <libnpln/disassembler/Table.hpp>

#include <algorithm>
#include <iterator>
#include <utility>

namespace libnpln::disassembler {

auto find_address(Table const& table, machine::Address addr) -> Table::const_iterator
{
    auto i = std::lower_bound(std::begin(table), std::end(table), addr);
    if (i == std::end(table) || !intersects(*i, addr)) {
        return std::end(table);
    }
    return i;
}

auto find_address(Table& table, machine::Address addr) -> Table::iterator
{
    auto i = std::lower_bound(std::begin(table), std::end(table), addr);
    if (i == std::end(table) || !intersects(*i, addr)) {
        return std::end(table);
    }
    return i;
}

auto find_label(Table const& table, std::string_view label) -> Table::const_iterator
{
    return std::find_if(
        std::begin(table), std::end(table), [&label](auto const& x) { return x.label == label; });
}

auto find_label(Table& table, std::string_view label) -> Table::iterator
{
    return std::find_if(
        std::begin(table), std::end(table), [&label](auto const& x) { return x.label == label; });
}

auto insert_row(Table& table, Row const& row) -> Table::iterator
{
    return insert_row(table, Row{row});
}

auto insert_row(Table& table, Row&& row) -> Table::iterator
{
    auto i = std::lower_bound(std::begin(table), std::end(table), row);
    if (i == std::end(table)) {
        table.push_back(std::move(row));
        return std::end(table);
    }
    if (intersects(*i, row)) {
        *i = std::move(row);
    }
    else {
        i = table.insert(i, std::move(row));
    }

    // Remove conflicting rows following the inserted row.
    auto j = std::next(i);
    if (j != std::end(table) && intersects(*i, *j)) {
        table.erase(j);
    }

    return i;
}

} // namespace libnpln::disassembler
