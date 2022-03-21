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

#include <libnpln/disassembler/Row.hpp>

#include <catch2/catch.hpp>

using namespace libnpln;
using namespace libnpln::disassembler;

TEST_CASE("Row can contain instruction data", "[disassembler][row]")
{
    auto const r = Row{
        0x000, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    REQUIRE(std::holds_alternative<machine::Instruction>(r.data));
}

TEST_CASE("Row can contain byte data", "[disassembler][row]")
{
    auto const r = Row{0x000, machine::Byte{0xAA}, "test"};
    REQUIRE(std::holds_alternative<machine::Byte>(r.data));
}

TEST_CASE("Row can compute its data width", "[disassembler][row]")
{
    SECTION("when it contains instruction data")
    {
        auto const r = Row{0x000,
            machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
        REQUIRE(r.data_width() == machine::Instruction::width);
    }

    SECTION("when it contains byte data")
    {
        auto const r = Row{0x000, machine::Byte{0xAA}, "test"};
        REQUIRE(r.data_width() == sizeof(machine::Byte));
    }
}

TEST_CASE("Row can compute its end address", "[disassembler][row]")
{
    SECTION("when it contains instruction data")
    {
        auto const r = Row{0x200,
            machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
        REQUIRE(r.end_address() == r.address + r.data_width());
    }

    SECTION("when it contains byte data")
    {
        auto const r = Row{0x200, machine::Byte{0xAA}, "test"};
        REQUIRE(r.end_address() == r.address + r.data_width());
    }
}

TEST_CASE("Row intersections can be computed", "[disassembler][row]")
{
    auto const r1 = Row{
        0x200, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    auto const r2 = Row{
        0x201, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    auto const r3 = Row{
        0x202, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    auto const r4 = Row{0x200, machine::Byte{0xAA}, "test"};
    auto const r5 = Row{0x201, machine::Byte{0xAA}, "test"};
    auto const r6 = Row{0x202, machine::Byte{0xAA}, "test"};

    SECTION("between two instruction rows")
    {
        // Reflexive
        REQUIRE(intersects(r1, r1));
        REQUIRE(intersects(r2, r2));
        REQUIRE(intersects(r3, r3));

        REQUIRE(intersects(r1, r2));
        REQUIRE(intersects(r2, r1)); // Commutative

        REQUIRE(!intersects(r1, r3));
        REQUIRE(!intersects(r3, r1)); // Commutative
    }

    SECTION("between two byte rows")
    {
        // Reflexive
        REQUIRE(intersects(r4, r4));
        REQUIRE(intersects(r5, r5));
        REQUIRE(intersects(r6, r6));
    }

    SECTION("between an instruction row and a byte row")
    {
        REQUIRE(intersects(r1, r4));
        REQUIRE(intersects(r4, r1)); // Commutative

        REQUIRE(intersects(r1, r5));
        REQUIRE(intersects(r5, r1)); // Commutative

        REQUIRE(!intersects(r1, r6));
        REQUIRE(!intersects(r6, r1)); // Commutative
    }
}

TEST_CASE("Row-address intersections can be computed", "[disassembler][row]")
{
    SECTION("with instruction rows")
    {
        auto const r = Row{0x200,
            machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
        REQUIRE(!intersects(r, 0x199));
        REQUIRE(intersects(r, 0x200));
        REQUIRE(intersects(r, 0x201));
        REQUIRE(!intersects(r, 0x202));
    }

    SECTION("with instruction rows")
    {
        auto const r = Row{0x200, machine::Byte{0xAA}, "test"};
        REQUIRE(!intersects(r, 0x199));
        REQUIRE(intersects(r, 0x200));
        REQUIRE(!intersects(r, 0x201));
    }
}

TEST_CASE("Rows can be compared for equality", "[disassembler][row]")
{
    auto const r1 = Row{0x200, machine::Instruction::decode(0x00EE).value(), {}};
    auto const r2 = Row{0x200, machine::Instruction::decode(0x00EE).value(), "foo"};
    auto const r3 = Row{0x200, machine::Byte{0xAA}, {}};
    auto const r4 = Row{0x200, machine::Byte{0xAA}, "foo"};
    auto const r5 = Row{0x201, machine::Instruction::decode(0x00EE).value(), {}};

    REQUIRE(r1 == r1);
    REQUIRE(r2 == r2);
    REQUIRE(r3 == r3);
    REQUIRE(r4 == r4);
    REQUIRE(r5 == r5);

    REQUIRE(r1 != r2);
    REQUIRE(r2 != r3);
    REQUIRE(r3 != r4);
    REQUIRE(r4 != r5);
    REQUIRE(r5 != r1);
}

TEST_CASE("Rows can be compared for order", "[disassembler][row]")
{
    auto const r1 = Row{
        0x200, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    auto const r2 = Row{
        0x201, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    auto const r3 = Row{
        0x202, machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
    auto const r4 = Row{0x200, machine::Byte{0xAA}, "test"};
    auto const r5 = Row{0x201, machine::Byte{0xAA}, "test"};
    auto const r6 = Row{0x202, machine::Byte{0xAA}, "test"};

    REQUIRE(r1 <= r1);
    REQUIRE(r1 >= r1);

    REQUIRE(!(r1 < r2));
    REQUIRE(r1 <= r2);
    REQUIRE(r1 < r3);
    REQUIRE(r1 <= r3);
    REQUIRE(!(r2 < r3));
    REQUIRE(r2 <= r3);

    REQUIRE(!(r2 > r1));
    REQUIRE(r2 >= r1);
    REQUIRE(r3 > r1);
    REQUIRE(r3 >= r1);
    REQUIRE(!(r3 > r2));
    REQUIRE(r3 >= r2);
}

TEST_CASE("Row can be formatted", "[disassembler][row]")
{
    SECTION("when it contains instruction data")
    {
        auto const r1 = Row{0x200,
            machine::Instruction{machine::Operator::cls, machine::NullaryOperands{}}, "test"};
        REQUIRE(fmt::format("{}", r1) == "test@200-202:Instruction(CLS)");

        auto const r2 = Row{
            0x302, machine::Instruction{machine::Operator::ret, machine::NullaryOperands{}}, {}};
        REQUIRE(fmt::format("{}", r2) == "@302-304:Instruction(RET)");
    }

    SECTION("when it contains byte data")
    {
        auto const r1 = Row{0x200, machine::Byte{0xAA}, "test"};
        REQUIRE(fmt::format("{}", r1) == "test@200-201:Byte(AA)");

        auto const r2 = Row{0x302, machine::Byte{0x55}, {}};
        REQUIRE(fmt::format("{}", r2) == "@302-303:Byte(55)");
    }
}
