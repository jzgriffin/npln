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

#ifndef LIBNPLN_MACHINE_OPERATOR_HPP
#define LIBNPLN_MACHINE_OPERATOR_HPP

#include <libnpln/detail/unreachable.hpp>
#include <libnpln/machine/DataUnits.hpp>

#include <fmt/format.h>

#include <string_view>

namespace libnpln::machine {

enum class Operator : Word
{
    cls = 0x00E0,
    ret = 0x00EE,
    jmp_a = 0x1000,
    call_a = 0x2000,
    seq_v_b = 0x3000,
    sne_v_b = 0x4000,
    seq_v_v = 0x5000,
    mov_v_b = 0x6000,
    add_v_b = 0x7000,
    mov_v_v = 0x8000,
    or_v_v = 0x8001,
    and_v_v = 0x8002,
    xor_v_v = 0x8003,
    add_v_v = 0x8004,
    sub_v_v = 0x8005,
    shr_v = 0x8006,
    subn_v_v = 0x8007,
    shl_v = 0x800E,
    sne_v_v = 0x9000,
    mov_i_a = 0xA000,
    jmp_v0_a = 0xB000,
    rnd_v_b = 0xC000,
    drw_v_v_n = 0xD000,
    skp_v = 0xE09E,
    sknp_v = 0xE0A1,
    mov_v_dt = 0xF007,
    wkp_v = 0xF00A,
    mov_dt_v = 0xF015,
    mov_st_v = 0xF018,
    add_i_v = 0xF01E,
    font_v = 0xF029,
    bcd_v = 0xF033,
    mov_ii_v = 0xF055,
    mov_v_ii = 0xF065,
};

constexpr auto get_format_string(Operator const op) -> std::string_view
{
    switch (op) {
        case Operator::cls: return "CLS";
        case Operator::ret: return "RET";
        case Operator::jmp_a: return "JMP {address}";
        case Operator::call_a: return "CALL {address}";
        case Operator::seq_v_b: return "SEQ %{Vx}, ${byte}";
        case Operator::sne_v_b: return "SNE %{Vx}, ${byte}";
        case Operator::seq_v_v: return "SEQ %{Vx}, %{Vy}";
        case Operator::mov_v_b: return "MOV ${byte}, %{Vx}";
        case Operator::add_v_b: return "ADD ${byte}, %{Vx}";
        case Operator::mov_v_v: return "MOV %{Vy}, %{Vx}";
        case Operator::or_v_v: return "OR %{Vy}, %{Vx}";
        case Operator::and_v_v: return "AND %{Vy}, %{Vx}";
        case Operator::xor_v_v: return "XOR %{Vy}, %{Vx}";
        case Operator::add_v_v: return "ADD %{Vy}, %{Vx}";
        case Operator::sub_v_v: return "SUB %{Vy}, %{Vx}";
        case Operator::shr_v: return "SHR %{Vx}";
        case Operator::subn_v_v: return "SUBN %{Vy}, %{Vx}";
        case Operator::shl_v: return "SHL %{Vx}";
        case Operator::sne_v_v: return "SNE %{Vx}, %{Vy}";
        case Operator::mov_i_a: return "MOV {address}, %I";
        case Operator::jmp_v0_a: return "JMP %V0({address})";
        case Operator::rnd_v_b: return "RND ${byte}, %{Vx}";
        case Operator::drw_v_v_n: return "DRW %{Vx}, %{Vy}, ${nibble}";
        case Operator::skp_v: return "SKP %{Vx}";
        case Operator::sknp_v: return "SKNP %{Vx}";
        case Operator::mov_v_dt: return "MOV %DT, %{Vx}";
        case Operator::wkp_v: return "WKP %{Vx}";
        case Operator::mov_dt_v: return "MOV %{Vx}, %DT";
        case Operator::mov_st_v: return "MOV %{Vx}, %ST";
        case Operator::add_i_v: return "ADD %{Vx}, %I";
        case Operator::font_v: return "FONT %{Vx}";
        case Operator::bcd_v: return "BCD %{Vx}";
        case Operator::mov_ii_v: return "MOV %V0..%{Vx}, (%I)";
        case Operator::mov_v_ii: return "MOV (%I), %V0..%{Vx}";
    }

    LIBNPLN_DETAIL_UNREACHABLE
}

}

template<>
struct fmt::formatter<libnpln::machine::Operator>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    // The string output by this function is itself a format string.  The
    // format string may be formatted using named arguments whose names match
    // the parameters in the operator's mnemonic.
    template<typename FormatContext>
    auto format(libnpln::machine::Operator const& value,
        FormatContext& context)
    {
        return format_to(context.out(), "{}",
            libnpln::machine::get_format_string(value));
    }
};

#endif
