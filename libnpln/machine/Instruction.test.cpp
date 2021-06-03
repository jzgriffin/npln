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

#include <libnpln/machine/Instruction.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("Instructions are of acceptable widths", "[machine][instruction]")
{
    REQUIRE(Instruction::width == 2 * sizeof(Byte));
}

// NOLINTNEXTLINE(readability-function-size, hicpp-function-size)
TEST_CASE("Instructions encode/decode correctly", "[machine][instruction]")
{
    SECTION("cls")
    {
        auto const x = Instruction{Operator::cls, NullaryOperands{}};
        REQUIRE(x.encode() == 0x00E0);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "CLS");
    }

    SECTION("ret")
    {
        auto const x = Instruction{Operator::ret, NullaryOperands{}};
        REQUIRE(x.encode() == 0x00EE);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "RET");
    }

    SECTION("jmp_a")
    {
        auto const x = Instruction{Operator::jmp_a, AOperands{0xFDB}};
        REQUIRE(x.encode() == 0x1FDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "JMP FDBh");
    }

    SECTION("call_a")
    {
        auto const x = Instruction{Operator::call_a, AOperands{0xFDB}};
        REQUIRE(x.encode() == 0x2FDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "CALL FDBh");
    }

    SECTION("seq_v_b")
    {
        auto const x = Instruction{Operator::seq_v_b, VBOperands{Register::vf, 0xDB}};
        REQUIRE(x.encode() == 0x3FDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SEQ %VF, $DBh");
    }

    SECTION("sne_v_b")
    {
        auto const x = Instruction{Operator::sne_v_b, VBOperands{Register::vf, 0xDB}};
        REQUIRE(x.encode() == 0x4FDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SNE %VF, $DBh");
    }

    SECTION("seq_v_v")
    {
        auto const x = Instruction{Operator::seq_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x5FD0);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SEQ %VF, %VD");
    }

    SECTION("mov_v_b")
    {
        auto const x = Instruction{Operator::mov_v_b, VBOperands{Register::vf, 0xDB}};
        REQUIRE(x.encode() == 0x6FDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV $DBh, %VF");
    }

    SECTION("add_v_b")
    {
        auto const x = Instruction{Operator::add_v_b, VBOperands{Register::vf, 0xDB}};
        REQUIRE(x.encode() == 0x7FDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "ADD $DBh, %VF");
    }

    SECTION("mov_v_v")
    {
        auto const x = Instruction{Operator::mov_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD0);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV %VD, %VF");
    }

    SECTION("or_v_v")
    {
        auto const x = Instruction{Operator::or_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD1);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "OR %VD, %VF");
    }

    SECTION("and_v_v")
    {
        auto const x = Instruction{Operator::and_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD2);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "AND %VD, %VF");
    }

    SECTION("xor_v_v")
    {
        auto const x = Instruction{Operator::xor_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD3);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "XOR %VD, %VF");
    }

    SECTION("add_v_v")
    {
        auto const x = Instruction{Operator::add_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD4);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "ADD %VD, %VF");
    }

    SECTION("sub_v_v")
    {
        auto const x = Instruction{Operator::sub_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD5);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SUB %VD, %VF");
    }

    SECTION("shr_v")
    {
        auto const x = Instruction{Operator::shr_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0x8F06);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SHR %VF");
    }

    SECTION("subn_v_v")
    {
        auto const x = Instruction{Operator::subn_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x8FD7);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SUBN %VD, %VF");
    }

    SECTION("shl_v")
    {
        auto const x = Instruction{Operator::shl_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0x8F0E);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SHL %VF");
    }

    SECTION("sne_v_v")
    {
        auto const x = Instruction{Operator::sne_v_v, VVOperands{Register::vf, Register::vd}};
        REQUIRE(x.encode() == 0x9FD0);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SNE %VF, %VD");
    }

    SECTION("mov_i_a")
    {
        auto const x = Instruction{Operator::mov_i_a, AOperands{0xFDB}};
        REQUIRE(x.encode() == 0xAFDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV FDBh, %I");
    }

    SECTION("jmp_v0_a")
    {
        auto const x = Instruction{Operator::jmp_v0_a, AOperands{0xFDB}};
        REQUIRE(x.encode() == 0xBFDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "JMP %V0(FDBh)");
    }

    SECTION("rnd_v_b")
    {
        auto const x = Instruction{Operator::rnd_v_b, VBOperands{Register::vf, 0xDB}};
        REQUIRE(x.encode() == 0xCFDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "RND $DBh, %VF");
    }

    SECTION("drw_v_v_n")
    {
        auto const x =
            Instruction{Operator::drw_v_v_n, VVNOperands{Register::vf, Register::vd, 0xB}};
        REQUIRE(x.encode() == 0xDFDB);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "DRW %VF, %VD, $Bh");
    }

    SECTION("skp_v")
    {
        auto const x = Instruction{Operator::skp_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xEF9E);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SKP %VF");
    }

    SECTION("sknp_v")
    {
        auto const x = Instruction{Operator::sknp_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xEFA1);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "SKNP %VF");
    }

    SECTION("mov_v_dt")
    {
        auto const x = Instruction{Operator::mov_v_dt, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF07);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV %DT, %VF");
    }

    SECTION("wkp_v")
    {
        auto const x = Instruction{Operator::wkp_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF0A);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "WKP %VF");
    }

    SECTION("mov_dt_v")
    {
        auto const x = Instruction{Operator::mov_dt_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF15);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV %VF, %DT");
    }

    SECTION("mov_st_v")
    {
        auto const x = Instruction{Operator::mov_st_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF18);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV %VF, %ST");
    }

    SECTION("add_i_v")
    {
        auto const x = Instruction{Operator::add_i_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF1E);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "ADD %VF, %I");
    }

    SECTION("font_v")
    {
        auto const x = Instruction{Operator::font_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF29);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "FONT %VF");
    }

    SECTION("bcd_v")
    {
        auto const x = Instruction{Operator::bcd_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF33);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "BCD %VF");
    }

    SECTION("mov_ii_v")
    {
        auto const x = Instruction{Operator::mov_ii_v, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF55);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV %V0..%VF, (%I)");
    }

    SECTION("mov_v_ii")
    {
        auto const x = Instruction{Operator::mov_v_ii, VOperands{Register::vf}};
        REQUIRE(x.encode() == 0xFF65);
        REQUIRE(x.decode(x.encode()) == x);
        REQUIRE(fmt::format("{}", x) == "MOV (%I), %V0..%VF");
    }
}
