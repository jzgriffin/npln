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

#include <stdexcept>

namespace {

using namespace libnpln::machine;

[[noreturn]] auto throw_unknown_instruction(Word const w) -> void
{
    throw std::runtime_error{fmt::format("Unknown instruction {:04X}", w)};
}

auto decode_f000(Word const w) -> Instruction
{
    auto op = static_cast<Operator>(w & 0xF000);
    switch (op) {
        case Operator::jmp_a:
        case Operator::call_a:
        case Operator::mov_i_a:
        case Operator::jmp_v0_a:
            return {op, AOperands::decode(w)};
        case Operator::seq_v_b:
        case Operator::sne_v_b:
        case Operator::mov_v_b:
        case Operator::add_v_b:
        case Operator::rnd_v_b:
            return {op, VBOperands::decode(w)};
        case Operator::drw_v_v_n:
            return {op, VVNOperands::decode(w)};
        default:
            throw_unknown_instruction(w);
    }
}

auto decode_f00f(Word const w) -> Instruction
{
    auto op = static_cast<Operator>(w & 0xF00F);
    switch (op) {
        case Operator::seq_v_v:
        case Operator::mov_v_v:
        case Operator::or_v_v:
        case Operator::and_v_v:
        case Operator::xor_v_v:
        case Operator::add_v_v:
        case Operator::sub_v_v:
        case Operator::subn_v_v:
        case Operator::sne_v_v:
            return {op, VVOperands::decode(w)};
        default:
            return decode_f000(w);
    }
}

auto decode_f0ff(Word const w) -> Instruction
{
    auto op = static_cast<Operator>(w & 0xF0FF);
    switch (op) {
        case Operator::shr_v:
        case Operator::shl_v:
        case Operator::skp_v:
        case Operator::sknp_v:
        case Operator::mov_v_dt:
        case Operator::wkp_v:
        case Operator::mov_dt_v:
        case Operator::mov_st_v:
        case Operator::add_i_v:
        case Operator::font_v:
        case Operator::bcd_v:
        case Operator::mov_ii_v:
        case Operator::mov_v_ii:
            return {op, VOperands::decode(w)};
        default:
            return decode_f00f(w);
    }
}

auto decode_ffff(Word const w) -> Instruction
{
    auto op = static_cast<Operator>(w);
    switch (op) {
        case Operator::cls:
        case Operator::ret:
            return {op, NullaryOperands::decode(w)};
        default:
            return decode_f0ff(w);
    }
}

}

namespace libnpln::machine {

auto Instruction::decode(Word const w) -> Instruction
{
    // The decode_* operations cascade until a matching decoding is found or
    // the possible decodings are exhausted, in which case an exception is
    // thrown.  The decode_ffff operation is the highest level of this cascade
    return decode_ffff(w);
}

auto Instruction::encode() const -> Word
{
    return static_cast<Word>(op) |
        std::visit([](auto&& a) { return a.encode(); }, args);
}

}
