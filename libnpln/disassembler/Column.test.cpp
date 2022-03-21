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

#include <libnpln/disassembler/Column.hpp>

#include <libnpln/detail/cpp2b.hpp>

#include <catch2/catch.hpp>

#include <iterator>

using namespace libnpln;
using namespace libnpln::disassembler;

namespace {

auto const columns = {
    Column::address,
    Column::opcode,
    Column::label,
    Column::instruction,
};

} // namespace

TEST_CASE("Column exposes correct enumerators", "[disassembler][column]")
{
    // Ensure that every column has a unique nonzero underlying value.
    for (auto i = std::begin(columns); i != std::end(columns); ++i) {
        REQUIRE(detail::to_underlying(*i) != 0);
        for (auto j = std::next(i); j != std::end(columns); ++j) {
            REQUIRE(detail::to_underlying(*i) != detail::to_underlying(*j));
        }
    }
}

TEST_CASE("Column exposes flag interface", "[disassembler][column]")
{
    static_assert(flags::is_flags<Column>::value);

    REQUIRE(no_columns.to_bitset().count() == 0);
    REQUIRE(
        all_columns.to_bitset().count() == std::distance(std::begin(columns), std::end(columns)));
}

TEST_CASE("Columns define names", "[disassembler][column]")
{
    REQUIRE(get_name(Column::address) == "Address");
    REQUIRE(get_name(Column::opcode) == "Opcode");
    REQUIRE(get_name(Column::label) == "Label");
    REQUIRE(get_name(Column::instruction) == "Instruction");
}

TEST_CASE("Unknown Columns do not define names", "[disassembler][column]")
{
    auto const invalid_column =
        static_cast<Column>(std::numeric_limits<std::underlying_type_t<Column>>::max());
    REQUIRE_THROWS_AS(get_name(invalid_column), std::out_of_range);
}

TEST_CASE("Column returns its name when formatted", "[disassembler][column]")
{
    for (auto&& c : columns) {
        REQUIRE(fmt::format("{}", c) == get_name(c));
    }
}
