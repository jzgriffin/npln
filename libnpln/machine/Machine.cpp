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
#include <libnpln/utility/Numeric.hpp>

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
    display.clear();
    return std::nullopt;
}

auto Machine::execute_ret(Address const pc, NullaryOperands const& args) noexcept -> Result
{
    auto const a = stack.pop();
    if (a == std::nullopt) {
        return Fault::Type::empty_stack;
    }

    program_counter = *a;
    return std::nullopt;
}

auto Machine::execute_jmp_a(Address const pc, AOperands const& args) noexcept -> Result
{
    program_counter = args.address;
    return std::nullopt;
}

auto Machine::execute_call_a(Address const pc, AOperands const& args) noexcept -> Result
{
    if (!stack.push(pc + sizeof(Word))) {
        return Fault::Type::full_stack;
    }

    program_counter = args.address;
    return std::nullopt;
}

auto Machine::execute_seq_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    if (registers[args.vx] == args.byte) {
        program_counter += sizeof(Word);
    }

    return std::nullopt;
}

auto Machine::execute_sne_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    if (registers[args.vx] != args.byte) {
        program_counter += sizeof(Word);
    }

    return std::nullopt;
}

auto Machine::execute_seq_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    if (registers[args.vx] == registers[args.vy]) {
        program_counter += sizeof(Word);
    }

    return std::nullopt;
}

auto Machine::execute_mov_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    registers[args.vx] = args.byte;
    return std::nullopt;
}

auto Machine::execute_add_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    registers[args.vx] += args.byte;
    return std::nullopt;
}

auto Machine::execute_mov_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    registers[args.vx] = registers[args.vy];
    return std::nullopt;
}

auto Machine::execute_or_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    registers[args.vx] |= registers[args.vy];
    return std::nullopt;
}

auto Machine::execute_and_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    registers[args.vx] &= registers[args.vy];
    return std::nullopt;
}

auto Machine::execute_xor_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    registers[args.vx] ^= registers[args.vy];
    return std::nullopt;
}

// The following instructions, which modify the %VF register to indicate some
// flag, must take the following special considerations into account because
// %VF can also be an operand register:
// - Using a reference to an operand register can result in aliasing.
//   Modifying %VF can also change the referenced value, affecting subsequent
//   operations on that register.
// - In-place operations on an operand register is also affected by aliasing.
//   After %VF is modified to set/clear the appropriate flag, if it is also
//   the destination register, any in-place operation on the destination
//   register will use the flag output rather than the original value.
// - The order in which operations are applied will change the output. If %VF
//   is set/cleared after the destination register is updated, its status as a
//   flag register will take priority over its status as the destination and
//   vice-versa.
// The test cases for these instructions are designed to verify that using %VF
// as an operand register takes precedence over its status as a flag register.

auto Machine::execute_add_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    auto const y = registers[args.vy];
    registers.vf = utility::addition_overflow(x, y); // Carry
    registers[args.vx] = x + y;
    return std::nullopt;
}

auto Machine::execute_sub_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    auto const y = registers[args.vy];
    registers.vf = !utility::subtraction_underflow(x, y); // Not borrow
    registers[args.vx] = x - y;
    return std::nullopt;
}

auto Machine::execute_shr_v(Address const pc, VOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    registers.vf = utility::lsb(x);
    registers[args.vx] = x >> 1;
    return std::nullopt;
}

auto Machine::execute_subn_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    auto const y = registers[args.vy];
    registers.vf = !utility::subtraction_underflow(y, x); // Not borrow
    registers[args.vx] = y - x;
    return std::nullopt;
}

auto Machine::execute_shl_v(Address const pc, VOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    registers.vf = utility::msb(x);
    registers[args.vx] = x << 1;
    return std::nullopt;
}

auto Machine::execute_sne_v_v(Address const pc, VVOperands const& args) noexcept -> Result
{
    if (registers[args.vx] != registers[args.vy]) {
        program_counter += sizeof(Word);
    }

    return std::nullopt;
}

auto Machine::execute_mov_i_a(Address const pc, AOperands const& args) noexcept -> Result
{
    registers.i = args.address;
    return std::nullopt;
}

auto Machine::execute_jmp_v0_a(Address const pc, AOperands const& args) noexcept -> Result
{
    program_counter = registers.v0 + args.address;
    return std::nullopt;
}

auto Machine::execute_rnd_v_b(Address const pc, VBOperands const& args) noexcept -> Result
{
    static std::uniform_int_distribution<Byte> byte_dist;

    registers[args.vx] = byte_dist(random_engine) & args.byte;
    return std::nullopt;
}

auto Machine::execute_drw_v_v_n(Address const pc, VVNOperands const& args) noexcept -> Result
{
    if (registers.i + args.nibble >= memory.size()) {
        return Fault::Type::invalid_address;
    }

    // Each byte of sprite data is drawn on its own row.
    // Each bit of sprite row data is a pixel.
    static constexpr auto row_bits = std::numeric_limits<Byte>::digits;
    auto x = registers[args.vx];
    auto y = registers[args.vy];
    registers.vf = false; // Pixel cleared
    for (std::size_t i = 0; i < args.nibble; ++i) {
        auto const y = registers[args.vy] + i;
        auto const a = registers.i + i;
        auto const row = memory[a];
        for (std::size_t j = 0; j < row_bits; ++j) {
            auto const x = registers[args.vx] + j;
            auto p = display.pixel(x, y);
            if (p == nullptr) {
                break; // Prevent drawing outside of the display
            }

            auto const bit = (row & (1 << (row_bits - j - 1))) != 0;
            if (*p && bit) {
                registers.vf = true; // Pixel cleared
            }
            *p = bit != *p;
        }
    }

    return std::nullopt;
}

auto Machine::execute_skp_v(Address const pc, VOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    if (x < keys.size() && keys.test(x)) { // Unknown keys are never pressed
        program_counter += sizeof(Word);
    }

    return std::nullopt;
}

auto Machine::execute_sknp_v(Address const pc, VOperands const& args) noexcept -> Result
{
    auto const x = registers[args.vx];
    if (x >= keys.size() || !keys.test(x)) { // Unknown keys are never pressed
        program_counter += sizeof(Word);
    }

    return std::nullopt;
}

auto Machine::execute_mov_v_dt(Address const pc, VOperands const& args) noexcept -> Result
{
    registers[args.vx] = registers.dt;
    return std::nullopt;
}

auto Machine::execute_wkp_v(Address const pc, VOperands const& args) noexcept -> Result
{
    // This instruction will repeat until a key is pressed.  If a key is
    // already pressed the first time this instruction runs, it will be
    // considered the pressed key.

    // Select the lowest key pressed.  If multiple keys are pressed, the key
    // to choose for this instruction is arbitrary.
    for (std::size_t i = 0; i < keys.size(); ++i) {
        if (keys.test(i)) {
            registers[args.vx] = static_cast<Byte>(i);
            return std::nullopt;
        }
    }

    program_counter = pc;
    return std::nullopt;
}

auto Machine::execute_mov_dt_v(Address const pc, VOperands const& args) noexcept -> Result
{
    registers.dt = registers[args.vx];
    return std::nullopt;
}

auto Machine::execute_mov_st_v(Address const pc, VOperands const& args) noexcept -> Result
{
    registers.st = registers[args.vx];
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
