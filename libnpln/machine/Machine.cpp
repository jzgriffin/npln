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

#include <libnpln/machine/Machine.hpp>

#include <libnpln/machine/Font.hpp>
#include <libnpln/machine/RegisterRange.hpp>
#include <libnpln/utility/Numeric.hpp>

#include <gsl/gsl>

#include <stdexcept>

namespace libnpln::machine {

Machine::Machine() : memory_(std::make_unique<Memory>())
{
    if (!load_font_into_memory(*memory_, font_address)) {
        throw std::logic_error{"Unable to load font into machine memory"};
    }
}

Machine::Machine(Machine const& other)
    : fault_(other.fault_)
    , program_counter_(other.program_counter_)
    , registers_(other.registers_)
    , stack_(other.stack_)
    , memory_(std::make_unique<Memory>(*other.memory_))
    , keys_(other.keys_)
    , display_(other.display_)
    , master_clock_rate_(other.master_clock_rate_)
    , delay_cycles(other.delay_cycles)
    , sound_cycles(other.sound_cycles)
    , random_engine(other.random_engine)
{}

Machine::Machine(Machine&& other) noexcept
    : fault_(other.fault_)
    , program_counter_(other.program_counter_)
    , registers_(other.registers_)
    , stack_(other.stack_)
    , memory_(std::move(other.memory_))
    , keys_(other.keys_)
    , display_(std::move(other.display_))
    , master_clock_rate_(other.master_clock_rate_)
    , delay_cycles(other.delay_cycles)
    , sound_cycles(other.sound_cycles)
    , random_engine(other.random_engine)
{}

auto Machine::operator=(Machine const& other) -> Machine&
{
    if (this == &other) {
        return *this;
    }

    fault_ = other.fault_;
    program_counter_ = other.program_counter_;
    registers_ = other.registers_;
    stack_ = other.stack_;
    *memory_ = *other.memory_;
    keys_ = other.keys_;
    display_ = other.display_;
    master_clock_rate_ = other.master_clock_rate_;
    delay_cycles = other.delay_cycles;
    sound_cycles = other.delay_cycles;
    return *this;
}

auto Machine::operator=(Machine&& other) noexcept -> Machine&
{
    if (this == &other) {
        return *this;
    }

    fault_ = other.fault_;
    program_counter_ = other.program_counter_;
    registers_ = other.registers_;
    stack_ = other.stack_;
    memory_ = std::move(other.memory_);
    keys_ = other.keys_;
    display_ = std::move(other.display_);
    master_clock_rate_ = other.master_clock_rate_;
    delay_cycles = other.delay_cycles;
    sound_cycles = other.delay_cycles;
    return *this;
}

auto Machine::cycle() -> bool
{
    if (fault_ != std::nullopt) {
        return false;
    }

    auto const iw = fetch();
    if (iw == std::nullopt) {
        fault_ = Fault{Fault::Type::invalid_address, program_counter_};
        return false;
    }

    auto const i = Instruction::decode(*iw);
    if (i == std::nullopt) {
        fault_ = Fault{Fault::Type::invalid_instruction, program_counter_};
        return false;
    }

    auto const ft = execute(*i);
    if (ft != std::nullopt) {
        fault_ = Fault{*ft, program_counter_};
        return false;
    }

    ++delay_cycles;
    if (master_clock_rate_ / delay_cycles <= delay_clock_rate) {
        delay_cycles = 0;
        if (registers_.dt > 0) {
            --registers_.dt;
        }
    }

    ++sound_cycles;
    if (master_clock_rate_ / sound_cycles <= sound_clock_rate) {
        sound_cycles = 0;
        if (registers_.st > 0) {
            --registers_.st;
        }
    }

    return true;
}

auto Machine::fetch() noexcept -> std::optional<Word>
{
    if (program_counter_ + 1 >= memory_->size()) {
        return std::nullopt;
    }

    auto const high = gsl::at(*memory_, program_counter_ + 0);
    auto const low = gsl::at(*memory_, program_counter_ + 1);
    return make_word(high, low); // Big-endian
}

auto Machine::execute(Instruction const& instr) -> Result
{
    switch (instr.op) {
    case Operator::cls: return execute_cls();
    case Operator::ret: return execute_ret();
    case Operator::jmp_a: return execute_jmp_a(std::get<AOperands>(instr.args));
    case Operator::call_a: return execute_call_a(std::get<AOperands>(instr.args));
    case Operator::seq_v_b: return execute_seq_v_b(std::get<VBOperands>(instr.args));
    case Operator::sne_v_b: return execute_sne_v_b(std::get<VBOperands>(instr.args));
    case Operator::seq_v_v: return execute_seq_v_v(std::get<VVOperands>(instr.args));
    case Operator::mov_v_b: return execute_mov_v_b(std::get<VBOperands>(instr.args));
    case Operator::add_v_b: return execute_add_v_b(std::get<VBOperands>(instr.args));
    case Operator::mov_v_v: return execute_mov_v_v(std::get<VVOperands>(instr.args));
    case Operator::or_v_v: return execute_or_v_v(std::get<VVOperands>(instr.args));
    case Operator::and_v_v: return execute_and_v_v(std::get<VVOperands>(instr.args));
    case Operator::xor_v_v: return execute_xor_v_v(std::get<VVOperands>(instr.args));
    case Operator::add_v_v: return execute_add_v_v(std::get<VVOperands>(instr.args));
    case Operator::sub_v_v: return execute_sub_v_v(std::get<VVOperands>(instr.args));
    case Operator::shr_v: return execute_shr_v(std::get<VOperands>(instr.args));
    case Operator::subn_v_v: return execute_subn_v_v(std::get<VVOperands>(instr.args));
    case Operator::shl_v: return execute_shl_v(std::get<VOperands>(instr.args));
    case Operator::sne_v_v: return execute_sne_v_v(std::get<VVOperands>(instr.args));
    case Operator::mov_i_a: return execute_mov_i_a(std::get<AOperands>(instr.args));
    case Operator::jmp_v0_a: return execute_jmp_v0_a(std::get<AOperands>(instr.args));
    case Operator::rnd_v_b: return execute_rnd_v_b(std::get<VBOperands>(instr.args));
    case Operator::drw_v_v_n: return execute_drw_v_v_n(std::get<VVNOperands>(instr.args));
    case Operator::skp_v: return execute_skp_v(std::get<VOperands>(instr.args));
    case Operator::sknp_v: return execute_sknp_v(std::get<VOperands>(instr.args));
    case Operator::mov_v_dt: return execute_mov_v_dt(std::get<VOperands>(instr.args));
    case Operator::wkp_v: return execute_wkp_v(std::get<VOperands>(instr.args));
    case Operator::mov_dt_v: return execute_mov_dt_v(std::get<VOperands>(instr.args));
    case Operator::mov_st_v: return execute_mov_st_v(std::get<VOperands>(instr.args));
    case Operator::add_i_v: return execute_add_i_v(std::get<VOperands>(instr.args));
    case Operator::font_v: return execute_font_v(std::get<VOperands>(instr.args));
    case Operator::bcd_v: return execute_bcd_v(std::get<VOperands>(instr.args));
    case Operator::mov_ii_v: return execute_mov_ii_v(std::get<VOperands>(instr.args));
    case Operator::mov_v_ii: return execute_mov_v_ii(std::get<VOperands>(instr.args));
    }

    throw std::out_of_range("Unknown Operator in Machine::execute");
}

auto Machine::execute_cls() -> Result
{
    display_.clear();

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_ret() -> Result
{
    auto const a = stack_.pop();
    if (a == std::nullopt) {
        return Fault::Type::empty_stack;
    }

    program_counter_ = *a;
    return std::nullopt;
}

auto Machine::execute_jmp_a(AOperands const& args) -> Result
{
    program_counter_ = args.address;
    return std::nullopt;
}

auto Machine::execute_call_a(AOperands const& args) -> Result
{
    if (!stack_.push(program_counter_ + Instruction::width)) {
        return Fault::Type::full_stack;
    }

    program_counter_ = args.address;
    return std::nullopt;
}

auto Machine::execute_seq_v_b(VBOperands const& args) -> Result
{
    if (registers_[args.vx] == args.byte) {
        program_counter_ += Instruction::width;
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_sne_v_b(VBOperands const& args) -> Result
{
    if (registers_[args.vx] != args.byte) {
        program_counter_ += Instruction::width;
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_seq_v_v(VVOperands const& args) -> Result
{
    if (registers_[args.vx] == registers_[args.vy]) {
        program_counter_ += Instruction::width;
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_v_b(VBOperands const& args) -> Result
{
    registers_[args.vx] = args.byte;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_add_v_b(VBOperands const& args) -> Result
{
    registers_[args.vx] += args.byte;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_v_v(VVOperands const& args) -> Result
{
    registers_[args.vx] = registers_[args.vy];

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_or_v_v(VVOperands const& args) -> Result
{
    registers_[args.vx] |= registers_[args.vy];

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_and_v_v(VVOperands const& args) -> Result
{
    registers_[args.vx] &= registers_[args.vy];

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_xor_v_v(VVOperands const& args) -> Result
{
    registers_[args.vx] ^= registers_[args.vy];

    program_counter_ += Instruction::width;
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

auto Machine::execute_add_v_v(VVOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    auto const y = registers_[args.vy];
    registers_.vf = utility::addition_overflow(x, y) ? 1U : 0U; // Carry
    registers_[args.vx] = x + y;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_sub_v_v(VVOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    auto const y = registers_[args.vy];
    registers_.vf = utility::subtraction_underflow(x, y) ? 0U : 1U; // Not borrow
    registers_[args.vx] = x - y;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_shr_v(VOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    registers_.vf = utility::lsb(x) ? 1U : 0U;
    registers_[args.vx] = x >> 1U;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_subn_v_v(VVOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    auto const y = registers_[args.vy];
    registers_.vf = utility::subtraction_underflow(y, x) ? 0U : 1U; // Not borrow
    registers_[args.vx] = y - x;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_shl_v(VOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    registers_.vf = utility::msb(x) ? 1U : 0U;
    registers_[args.vx] = x << 1U;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_sne_v_v(VVOperands const& args) -> Result
{
    if (registers_[args.vx] != registers_[args.vy]) {
        program_counter_ += Instruction::width;
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_i_a(AOperands const& args) -> Result
{
    registers_.i = args.address;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_jmp_v0_a(AOperands const& args) -> Result
{
    program_counter_ = registers_.v0 + args.address;
    return std::nullopt;
}

auto Machine::execute_rnd_v_b(VBOperands const& args) -> Result
{
    static std::uniform_int_distribution<std::size_t> byte_dist{
        std::numeric_limits<Byte>::min(),
        std::numeric_limits<Byte>::max(),
    };

    registers_[args.vx] = static_cast<Byte>(byte_dist(random_engine) & args.byte);

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_drw_v_v_n(VVNOperands const& args) -> Result
{
    if (args.nibble > max_nibble) {
        return Fault::Type::invalid_instruction;
    }

    if (registers_.i + args.nibble >= memory_->size()) {
        return Fault::Type::invalid_address;
    }

    // Each byte of sprite data is drawn on its own row.
    // Each bit of sprite row data is a pixel.
    static constexpr auto row_bits = std::numeric_limits<Byte>::digits;
    auto const x0 = registers_[args.vx];
    auto const y0 = registers_[args.vy];
    registers_.vf = 0U; // Pixel cleared
    for (std::size_t i = 0; i < args.nibble; ++i) {
        auto const y = y0 + i;
        auto const a = registers_.i + i;
        auto const row = gsl::at(*memory_, static_cast<gsl::index>(a));
        for (std::size_t j = 0; j < row_bits; ++j) {
            auto const x = x0 + j;
            auto* p = display_.pixel(x, y);
            if (p == nullptr) {
                break; // Prevent drawing outside of the display_
            }

            auto const bit = (row & (1U << (row_bits - j - 1))) != 0;
            if (*p && bit) {
                registers_.vf = 1U; // Pixel cleared
            }
            *p = bit != *p;
        }
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_skp_v(VOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    if (x < keys_.size() && keys_.test(x)) { // Unknown keys_ are never pressed
        program_counter_ += sizeof(Word);
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_sknp_v(VOperands const& args) -> Result
{
    auto const x = registers_[args.vx];
    if (x >= keys_.size() || !keys_.test(x)) { // Unknown keys_ are never pressed
        program_counter_ += sizeof(Word);
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_v_dt(VOperands const& args) -> Result
{
    registers_[args.vx] = registers_.dt;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_wkp_v(VOperands const& args) -> Result
{
    // This instruction will repeat until a key is pressed.  If a key is
    // already pressed the first time this instruction runs, it will be
    // considered the pressed key.

    // Select the lowest key pressed.  If multiple keys_ are pressed, the key
    // to choose for this instruction is arbitrary.
    for (std::size_t i = 0; i < keys_.size(); ++i) {
        if (keys_.test(i)) {
            registers_[args.vx] = static_cast<Byte>(i);

            program_counter_ += Instruction::width;
            return std::nullopt;
        }
    }

    return std::nullopt;
}

auto Machine::execute_mov_dt_v(VOperands const& args) -> Result
{
    registers_.dt = registers_[args.vx];

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_st_v(VOperands const& args) -> Result
{
    registers_.st = registers_[args.vx];

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_add_i_v(VOperands const& args) -> Result
{
    registers_.i += registers_[args.vx];
    registers_.i &= 0xFFFU;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_font_v(VOperands const& args) -> Result
{
    auto const offset = get_glyph_offset(registers_[args.vx]);
    if (offset == std::nullopt) {
        return Fault::Type::invalid_digit;
    }

    registers_.i = font_address + *offset;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_bcd_v(VOperands const& args) -> Result
{
    if (registers_.i + 2 >= memory_->size()) {
        return Fault::Type::invalid_address;
    }

    auto const x = registers_[args.vx];
    // TODO: Refactor this into a more general algorithm
    gsl::at(*memory_, registers_.i + 0) = x / 100;
    gsl::at(*memory_, registers_.i + 1) = (x % 100) / 10;
    gsl::at(*memory_, registers_.i + 2) = ((x % 100) % 10) / 1;

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_ii_v(VOperands const& args) -> Result
{
    auto const rs = RegisterRange{args.vx};
    auto const d = std::distance(std::begin(rs), std::end(rs));

    if (registers_.i + d >= memory_->size()) {
        return Fault::Type::invalid_address;
    }

    std::transform(std::begin(rs), std::end(rs), std::next(std::begin(*memory_), registers_.i),
        [this](Register const r) { return registers_[r]; });

    program_counter_ += Instruction::width;
    return std::nullopt;
}

auto Machine::execute_mov_v_ii(VOperands const& args) -> Result
{
    auto const rs = RegisterRange{args.vx};
    auto const d = std::distance(std::begin(rs), std::end(rs));

    if (registers_.i + d >= memory_->size()) {
        return Fault::Type::invalid_address;
    }

    auto i = std::next(std::begin(*memory_), registers_.i);
    for (auto&& r : rs) {
        registers_[r] = *i++;
    }

    program_counter_ += Instruction::width;
    return std::nullopt;
}

} // namespace libnpln::machine
