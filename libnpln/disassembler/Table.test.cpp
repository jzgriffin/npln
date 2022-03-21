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

#include <libnpln/disassembler/TextRenderer.hpp>

#include <catch2/catch.hpp>

using namespace libnpln;
using namespace libnpln::disassembler;

// When constructing test tables, ensure that the addresses are ordered.

TEST_CASE("Table can find by aligned address", "[disassembler][table]")
{
    auto const t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };

    for (auto&& r : t) {
        auto const i = find_address(t, r.address);
        REQUIRE(i != std::end(t));
        REQUIRE(*i == r);
    }
}

TEST_CASE("Table can find by unaligned address", "[disassembler][table]")
{
    auto const t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };

    for (auto&& r : t) {
        if (!std::holds_alternative<machine::Instruction>(r.data)) {
            continue;
        }

        for (auto a = r.address; a < r.end_address(); ++a) {
            auto const i = find_address(t, a);
            REQUIRE(i != std::end(t));
            REQUIRE(*i == r);
        }
    }
}

TEST_CASE("Table cannot find by non-existent address", "[disassembler][table]")
{
    auto const t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };

    REQUIRE(find_address(t, 0x100) == std::end(t));
    REQUIRE(find_address(t, 0x204) == std::end(t));
    REQUIRE(find_address(t, 0x400) == std::end(t));
}

TEST_CASE("Table can find by label", "[disassembler][table]")
{
    auto const t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), "foo"},
        Row{0x202, machine::Byte{0xAA}, "bar"},
        Row{0x203, machine::Byte{0x55}, "baz"},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), "qux"},
    };

    for (auto&& r : t) {
        auto const i = find_label(t, r.label);
        REQUIRE(i != std::end(t));
        REQUIRE(*i == r);
    }
}

TEST_CASE("Table cannot find by non-existent label", "[disassembler][table]")
{
    auto const t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), "foo"},
        Row{0x202, machine::Byte{0xAA}, "bar"},
        Row{0x203, machine::Byte{0x55}, "baz"},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), "qux"},
    };

    REQUIRE(find_label(t, "") == std::end(t));
    REQUIRE(find_label(t, "A") == std::end(t));
    REQUIRE(find_label(t, "fo") == std::end(t));
}

TEST_CASE("Table returns first matching label", "[disassembler][table]")
{
    auto const t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), "foo"},
        Row{0x202, machine::Byte{0xAA}, "bar"},
        Row{0x203, machine::Byte{0x55}, "baz"},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), "foo"},
    };

    auto const i = find_label(t, "foo");
    REQUIRE(i != std::end(t));
    REQUIRE(t[0] == *i);
}

TEST_CASE("Table insertion creates rows at new addresses", "[disassembler][table]")
{
    auto t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };
    auto const t_size = t.size();

    auto const r = Row{0x250, machine::Byte{0xEE}, {}};
    auto const i = insert_row(t, r);
    REQUIRE(i == find_address(t, r.address));
    REQUIRE(t.size() == t_size + 1);
}

TEST_CASE("Table insertion overwrites rows at existing addresses", "[disassembler][table]")
{
    auto t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };
    auto const t_size = t.size();

    auto const r = Row{0x200, machine::Byte{0xEE}, {}};
    auto const i = insert_row(t, r);
    REQUIRE(i == find_address(t, r.address));
    REQUIRE(t.size() == t_size);
}

TEST_CASE("Table insertion does not create conflicts", "[disassembler][table]")
{
    auto is_consistent = [](auto const& t) {
        for (auto i = std::begin(t); i != std::end(t); ++i) {
            auto j = std::next(i);
            if (j == std::end(t)) {
                break;
            }

            if (intersects(*i, *j)) {
                return false;
            }
        }

        return true;
    };

    auto t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };
    auto const t_size = t.size();

    auto const r = Row{0x201, machine::Instruction::decode(0x00EE).value(), {}};
    auto const i = insert_row(t, r);
    REQUIRE(i == find_address(t, r.address));
    REQUIRE(is_consistent(t));
}

TEST_CASE("Table insertion preserves ordering", "[disassembler][table]")
{
    auto is_sorted = [](auto const& t0) {
        auto t1 = t0;
        std::sort(std::begin(t1), std::end(t1));
        return t0 == t1;
    };

    auto t = Table{
        Row{0x200, machine::Instruction::decode(0x00E0).value(), {}},
        Row{0x202, machine::Byte{0xAA}, {}},
        Row{0x203, machine::Byte{0x55}, {}},
        Row{0x300, machine::Instruction::decode(0x00EE).value(), {}},
    };

    REQUIRE(is_sorted(t));

    insert_row(t, Row{0x100, machine::Byte{0xEE}, {}});
    REQUIRE(is_sorted(t));

    insert_row(t, Row{0x200, machine::Byte{0xFF}, {}});
    REQUIRE(is_sorted(t));

    insert_row(t, Row{0x210, machine::Byte{0x11}, {}});
    REQUIRE(is_sorted(t));

    insert_row(t, Row{0x400, machine::Byte{0x33}, {}});
    REQUIRE(is_sorted(t));
}
