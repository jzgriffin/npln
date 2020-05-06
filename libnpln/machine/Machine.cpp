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

#include <libnpln/machine/Machine.hpp>

#include <libnpln/detail/unreachable.hpp>

namespace libnpln::machine {

auto Machine::cycle() noexcept -> bool
{
    if (fault != std::nullopt) {
        return false;
    }

    auto const pc = program_counter;
    auto const iw = fetch();
    if (iw == std::nullopt) {
        fault = Fault{Fault::Type::invalid_address, pc};
        return false;
    }

    auto const i = Instruction::decode(*iw);
    if (i == std::nullopt) {
        fault = Fault{Fault::Type::invalid_instruction, pc};
        return false;
    }

    auto const ft = execute(pc, *i);
    if (ft != std::nullopt) {
        fault = Fault{*ft, pc};
        return false;
    }

    return true;
}

auto Machine::fetch() noexcept -> std::optional<Word>
{
    if (program_counter + 1 >= memory.size()) {
        return std::nullopt;
    }

    auto const pcb = &memory[program_counter];
    program_counter += sizeof(Word);
    return make_word(*pcb, *(pcb + 1)); // Big-endian
}

auto Machine::execute(Address const pc, Instruction const& instr) noexcept -> Result
{
    switch (instr.op) {
        case Operator::cls: return execute_cls(pc, std::get<NullaryOperands>(instr.args));
        case Operator::ret: return execute_ret(pc, std::get<NullaryOperands>(instr.args));
        case Operator::jmp_a: return execute_jmp_a(pc, std::get<AOperands>(instr.args));
        case Operator::call_a: return execute_call_a(pc, std::get<AOperands>(instr.args));
        case Operator::seq_v_b: return execute_seq_v_b(pc, std::get<VBOperands>(instr.args));
        case Operator::sne_v_b: return execute_sne_v_b(pc, std::get<VBOperands>(instr.args));
        case Operator::seq_v_v: return execute_seq_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::mov_v_b: return execute_mov_v_b(pc, std::get<VBOperands>(instr.args));
        case Operator::add_v_b: return execute_add_v_b(pc, std::get<VBOperands>(instr.args));
        case Operator::mov_v_v: return execute_mov_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::or_v_v: return execute_or_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::and_v_v: return execute_and_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::xor_v_v: return execute_xor_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::add_v_v: return execute_add_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::sub_v_v: return execute_sub_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::shr_v: return execute_shr_v(pc, std::get<VOperands>(instr.args));
        case Operator::subn_v_v: return execute_subn_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::shl_v: return execute_shl_v(pc, std::get<VOperands>(instr.args));
        case Operator::sne_v_v: return execute_sne_v_v(pc, std::get<VVOperands>(instr.args));
        case Operator::mov_i_a: return execute_mov_i_a(pc, std::get<AOperands>(instr.args));
        case Operator::jmp_v0_a: return execute_jmp_v0_a(pc, std::get<AOperands>(instr.args));
        case Operator::rnd_v_b: return execute_rnd_v_b(pc, std::get<VBOperands>(instr.args));
        case Operator::drw_v_v_n: return execute_drw_v_v_n(pc, std::get<VVNOperands>(instr.args));
        case Operator::skp_v: return execute_skp_v(pc, std::get<VOperands>(instr.args));
        case Operator::sknp_v: return execute_sknp_v(pc, std::get<VOperands>(instr.args));
        case Operator::mov_v_dt: return execute_mov_v_dt(pc, std::get<VOperands>(instr.args));
        case Operator::wkp_v: return execute_wkp_v(pc, std::get<VOperands>(instr.args));
        case Operator::mov_dt_v: return execute_mov_dt_v(pc, std::get<VOperands>(instr.args));
        case Operator::mov_st_v: return execute_mov_st_v(pc, std::get<VOperands>(instr.args));
        case Operator::add_i_v: return execute_add_i_v(pc, std::get<VOperands>(instr.args));
        case Operator::font_v: return execute_font_v(pc, std::get<VOperands>(instr.args));
        case Operator::bcd_v: return execute_bcd_v(pc, std::get<VOperands>(instr.args));
        case Operator::mov_ii_v: return execute_mov_ii_v(pc, std::get<VOperands>(instr.args));
        case Operator::mov_v_ii: return execute_mov_v_ii(pc, std::get<VOperands>(instr.args));
    }

    LIBNPLN_DETAIL_UNREACHABLE
}

auto Machine::execute_cls(Address const pc, NullaryOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_ret(Address const pc, NullaryOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_jmp_a(Address const pc, AOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_call_a(Address const pc, AOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_seq_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_sne_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_seq_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_add_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_or_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_and_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_xor_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_add_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_sub_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_shr_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_subn_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_shl_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_sne_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_i_a(Address const pc, AOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_jmp_v0_a(Address const pc, AOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_rnd_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_drw_v_v_n(Address const pc, VVNOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_skp_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_sknp_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_v_dt(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_wkp_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_dt_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_st_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_add_i_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_font_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_bcd_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_ii_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

auto Machine::execute_mov_v_ii(Address const pc, VOperands const& args) noexcept -> Result
{
    // TODO
    return std::nullopt;
}

}
