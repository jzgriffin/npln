// Copyright 2020-2021 Jeremiah Griffin
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

#include <libnpln/detail/cpp2b.hpp>
#include <libnpln/machine/Operator.hpp>

#include <catch2/catch.hpp>

#include <limits>
#include <stdexcept>
#include <type_traits>

using namespace libnpln;
using namespace libnpln::machine;

TEST_CASE("Operators meet CHIP-8 specifications", "[machine][operator]")
{
    REQUIRE(detail::to_underlying(Operator::cls) == 0x00E0);
    REQUIRE(detail::to_underlying(Operator::ret) == 0x00EE);
    REQUIRE(detail::to_underlying(Operator::jmp_a) == 0x1000);
    REQUIRE(detail::to_underlying(Operator::call_a) == 0x2000);
    REQUIRE(detail::to_underlying(Operator::seq_v_b) == 0x3000);
    REQUIRE(detail::to_underlying(Operator::sne_v_b) == 0x4000);
    REQUIRE(detail::to_underlying(Operator::seq_v_v) == 0x5000);
    REQUIRE(detail::to_underlying(Operator::mov_v_b) == 0x6000);
    REQUIRE(detail::to_underlying(Operator::add_v_b) == 0x7000);
    REQUIRE(detail::to_underlying(Operator::mov_v_v) == 0x8000);
    REQUIRE(detail::to_underlying(Operator::or_v_v) == 0x8001);
    REQUIRE(detail::to_underlying(Operator::and_v_v) == 0x8002);
    REQUIRE(detail::to_underlying(Operator::xor_v_v) == 0x8003);
    REQUIRE(detail::to_underlying(Operator::add_v_v) == 0x8004);
    REQUIRE(detail::to_underlying(Operator::sub_v_v) == 0x8005);
    REQUIRE(detail::to_underlying(Operator::shr_v) == 0x8006);
    REQUIRE(detail::to_underlying(Operator::subn_v_v) == 0x8007);
    REQUIRE(detail::to_underlying(Operator::shl_v) == 0x800E);
    REQUIRE(detail::to_underlying(Operator::sne_v_v) == 0x9000);
    REQUIRE(detail::to_underlying(Operator::mov_i_a) == 0xA000);
    REQUIRE(detail::to_underlying(Operator::jmp_v0_a) == 0xB000);
    REQUIRE(detail::to_underlying(Operator::rnd_v_b) == 0xC000);
    REQUIRE(detail::to_underlying(Operator::drw_v_v_n) == 0xD000);
    REQUIRE(detail::to_underlying(Operator::skp_v) == 0xE09E);
    REQUIRE(detail::to_underlying(Operator::sknp_v) == 0xE0A1);
    REQUIRE(detail::to_underlying(Operator::mov_v_dt) == 0xF007);
    REQUIRE(detail::to_underlying(Operator::wkp_v) == 0xF00A);
    REQUIRE(detail::to_underlying(Operator::mov_dt_v) == 0xF015);
    REQUIRE(detail::to_underlying(Operator::mov_st_v) == 0xF018);
    REQUIRE(detail::to_underlying(Operator::add_i_v) == 0xF01E);
    REQUIRE(detail::to_underlying(Operator::font_v) == 0xF029);
    REQUIRE(detail::to_underlying(Operator::bcd_v) == 0xF033);
    REQUIRE(detail::to_underlying(Operator::mov_ii_v) == 0xF055);
    REQUIRE(detail::to_underlying(Operator::mov_v_ii) == 0xF065);
}

TEST_CASE("Operators define format strings", "[machine][operator]")
{
    REQUIRE(get_format_string(Operator::cls) == "CLS");
    REQUIRE(get_format_string(Operator::ret) == "RET");
    REQUIRE(get_format_string(Operator::jmp_a) == "JMP {address}");
    REQUIRE(get_format_string(Operator::call_a) == "CALL {address}");
    REQUIRE(get_format_string(Operator::seq_v_b) == "SEQ %{Vx}, ${byte}");
    REQUIRE(get_format_string(Operator::sne_v_b) == "SNE %{Vx}, ${byte}");
    REQUIRE(get_format_string(Operator::seq_v_v) == "SEQ %{Vx}, %{Vy}");
    REQUIRE(get_format_string(Operator::mov_v_b) == "MOV ${byte}, %{Vx}");
    REQUIRE(get_format_string(Operator::add_v_b) == "ADD ${byte}, %{Vx}");
    REQUIRE(get_format_string(Operator::mov_v_v) == "MOV %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::or_v_v) == "OR %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::and_v_v) == "AND %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::xor_v_v) == "XOR %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::add_v_v) == "ADD %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::sub_v_v) == "SUB %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::shr_v) == "SHR %{Vx}");
    REQUIRE(get_format_string(Operator::subn_v_v) == "SUBN %{Vy}, %{Vx}");
    REQUIRE(get_format_string(Operator::shl_v) == "SHL %{Vx}");
    REQUIRE(get_format_string(Operator::sne_v_v) == "SNE %{Vx}, %{Vy}");
    REQUIRE(get_format_string(Operator::mov_i_a) == "MOV {address}, %I");
    REQUIRE(get_format_string(Operator::jmp_v0_a) == "JMP %V0({address})");
    REQUIRE(get_format_string(Operator::rnd_v_b) == "RND ${byte}, %{Vx}");
    REQUIRE(get_format_string(Operator::drw_v_v_n) == "DRW %{Vx}, %{Vy}, ${nibble}");
    REQUIRE(get_format_string(Operator::skp_v) == "SKP %{Vx}");
    REQUIRE(get_format_string(Operator::sknp_v) == "SKNP %{Vx}");
    REQUIRE(get_format_string(Operator::mov_v_dt) == "MOV %DT, %{Vx}");
    REQUIRE(get_format_string(Operator::wkp_v) == "WKP %{Vx}");
    REQUIRE(get_format_string(Operator::mov_dt_v) == "MOV %{Vx}, %DT");
    REQUIRE(get_format_string(Operator::mov_st_v) == "MOV %{Vx}, %ST");
    REQUIRE(get_format_string(Operator::add_i_v) == "ADD %{Vx}, %I");
    REQUIRE(get_format_string(Operator::font_v) == "FONT %{Vx}");
    REQUIRE(get_format_string(Operator::bcd_v) == "BCD %{Vx}");
    REQUIRE(get_format_string(Operator::mov_ii_v) == "MOV %V0..%{Vx}, (%I)");
    REQUIRE(get_format_string(Operator::mov_v_ii) == "MOV (%I), %V0..%{Vx}");
}

TEST_CASE("Unknown Operators do not define format strings", "[machine][operator]")
{
    auto const invalid_operator =
        static_cast<Operator>(std::numeric_limits<std::underlying_type_t<Operator>>::max());
    REQUIRE_THROWS_AS(get_format_string(invalid_operator), std::out_of_range);
}

TEST_CASE("Operators return a format string when formatted", "[machine][operator]")
{
    REQUIRE(fmt::format("{}", Operator::cls) == get_format_string(Operator::cls));
    REQUIRE(fmt::format("{}", Operator::ret) == get_format_string(Operator::ret));
    REQUIRE(fmt::format("{}", Operator::jmp_a) == get_format_string(Operator::jmp_a));
    REQUIRE(fmt::format("{}", Operator::call_a) == get_format_string(Operator::call_a));
    REQUIRE(fmt::format("{}", Operator::seq_v_b) == get_format_string(Operator::seq_v_b));
    REQUIRE(fmt::format("{}", Operator::sne_v_b) == get_format_string(Operator::sne_v_b));
    REQUIRE(fmt::format("{}", Operator::seq_v_v) == get_format_string(Operator::seq_v_v));
    REQUIRE(fmt::format("{}", Operator::mov_v_b) == get_format_string(Operator::mov_v_b));
    REQUIRE(fmt::format("{}", Operator::add_v_b) == get_format_string(Operator::add_v_b));
    REQUIRE(fmt::format("{}", Operator::mov_v_v) == get_format_string(Operator::mov_v_v));
    REQUIRE(fmt::format("{}", Operator::or_v_v) == get_format_string(Operator::or_v_v));
    REQUIRE(fmt::format("{}", Operator::and_v_v) == get_format_string(Operator::and_v_v));
    REQUIRE(fmt::format("{}", Operator::xor_v_v) == get_format_string(Operator::xor_v_v));
    REQUIRE(fmt::format("{}", Operator::add_v_v) == get_format_string(Operator::add_v_v));
    REQUIRE(fmt::format("{}", Operator::sub_v_v) == get_format_string(Operator::sub_v_v));
    REQUIRE(fmt::format("{}", Operator::shr_v) == get_format_string(Operator::shr_v));
    REQUIRE(fmt::format("{}", Operator::subn_v_v) == get_format_string(Operator::subn_v_v));
    REQUIRE(fmt::format("{}", Operator::shl_v) == get_format_string(Operator::shl_v));
    REQUIRE(fmt::format("{}", Operator::sne_v_v) == get_format_string(Operator::sne_v_v));
    REQUIRE(fmt::format("{}", Operator::mov_i_a) == get_format_string(Operator::mov_i_a));
    REQUIRE(fmt::format("{}", Operator::jmp_v0_a) == get_format_string(Operator::jmp_v0_a));
    REQUIRE(fmt::format("{}", Operator::rnd_v_b) == get_format_string(Operator::rnd_v_b));
    REQUIRE(fmt::format("{}", Operator::drw_v_v_n) == get_format_string(Operator::drw_v_v_n));
    REQUIRE(fmt::format("{}", Operator::skp_v) == get_format_string(Operator::skp_v));
    REQUIRE(fmt::format("{}", Operator::sknp_v) == get_format_string(Operator::sknp_v));
    REQUIRE(fmt::format("{}", Operator::mov_v_dt) == get_format_string(Operator::mov_v_dt));
    REQUIRE(fmt::format("{}", Operator::wkp_v) == get_format_string(Operator::wkp_v));
    REQUIRE(fmt::format("{}", Operator::mov_dt_v) == get_format_string(Operator::mov_dt_v));
    REQUIRE(fmt::format("{}", Operator::mov_st_v) == get_format_string(Operator::mov_st_v));
    REQUIRE(fmt::format("{}", Operator::add_i_v) == get_format_string(Operator::add_i_v));
    REQUIRE(fmt::format("{}", Operator::font_v) == get_format_string(Operator::font_v));
    REQUIRE(fmt::format("{}", Operator::bcd_v) == get_format_string(Operator::bcd_v));
    REQUIRE(fmt::format("{}", Operator::mov_ii_v) == get_format_string(Operator::mov_ii_v));
    REQUIRE(fmt::format("{}", Operator::mov_v_ii) == get_format_string(Operator::mov_v_ii));
}
