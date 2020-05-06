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

#include <catch2/catch.hpp>

using namespace libnpln::machine;

template<>
struct Catch::StringMaker<Machine>
{
    static auto convert(Machine const& m)
    {
        return fmt::to_string(m);
    }
};

// By inspecting the entire state of the machine after each cycle, we verify
// that no instruction has an unintended side-effect.

TEST_CASE("Cycles fail after a fault", "[machine][cycle]")
{
    Machine m;
    m.fault = Fault{Fault::Type::invalid_instruction, m.program_counter};

    auto m_expect = m;

    CHECK_FALSE(m.cycle());
    REQUIRE(m == m_expect);
}

TEST_CASE("Cycles can resume after clearing a fault", "[machine][cycle]")
{
    Machine m;
    m.memory = create_program({
        0x80, 0x08,
        0x00, 0xE0, // CLS
    });

    auto m_expect = m;
    m_expect.fault = Fault{Fault::Type::invalid_instruction, m.program_counter};
    m_expect.program_counter += sizeof(Word);

    CHECK_FALSE(m.cycle());
    REQUIRE(m == m_expect);

    m.fault = std::nullopt;

    m_expect = m;
    m_expect.program_counter += sizeof(Word);

    CHECK(m.cycle());
    REQUIRE(m == m_expect);
}

TEST_CASE("Invalid addresses trigger a fault", "[machine][cycle]")
{
    Machine m;
    m.program_counter = 0x1000;

    auto m_expect = m;
    m_expect.fault = Fault{Fault::Type::invalid_address, m.program_counter};

    CHECK_FALSE(m.cycle());
    REQUIRE(m == m_expect);
}

TEST_CASE("Invalid instructions trigger a fault", "[machine][cycle]")
{
    Machine m;
    m.memory = create_program({
        0x00, 0x00,
    });

    auto m_expect = m;
    m_expect.fault = Fault{Fault::Type::invalid_instruction, m.program_counter};
    m_expect.program_counter += sizeof(Word);

    CHECK_FALSE(m.cycle());
    REQUIRE(m == m_expect);
}

TEST_CASE("Individual instructions execute correctly", "[machine][cycle]")
{
    // TODO: cls = 0x00E0,
    // TODO: ret = 0x00EE,
    // TODO: jmp_a = 0x1000,
    // TODO: call_a = 0x2000,
    // TODO: seq_v_b = 0x3000,
    // TODO: sne_v_b = 0x4000,
    // TODO: seq_v_v = 0x5000,
    // TODO: mov_v_b = 0x6000,
    // TODO: add_v_b = 0x7000,
    // TODO: mov_v_v = 0x8000,
    // TODO: or_v_v = 0x8001,
    // TODO: and_v_v = 0x8002,
    // TODO: xor_v_v = 0x8003,
    // TODO: add_v_v = 0x8004,
    // TODO: sub_v_v = 0x8005,
    // TODO: shr_v = 0x8006,
    // TODO: subn_v_v = 0x8007,
    // TODO: shl_v = 0x800E,
    // TODO: sne_v_v = 0x9000,
    // TODO: mov_i_a = 0xA000,
    // TODO: jmp_v0_a = 0xB000,
    // TODO: rnd_v_b = 0xC000,
    // TODO: drw_v_v_n = 0xD000,
    // TODO: skp_v = 0xE09E,
    // TODO: sknp_v = 0xE0A1,
    // TODO: mov_v_dt = 0xF007,
    // TODO: wkp_v = 0xF00A,
    // TODO: mov_dt_v = 0xF015,
    // TODO: mov_st_v = 0xF018,
    // TODO: add_i_v = 0xF01E,
    // TODO: font_v = 0xF029,
    // TODO: bcd_v = 0xF033,
    // TODO: mov_ii_v = 0xF055,
    // TODO: mov_v_ii = 0xF065,
}
