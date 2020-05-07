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

namespace {

auto create_checkerboard() -> Display
{
    Display d;
    for (std::size_t y = 0; y < d.height; ++y) {
        for (std::size_t x = 0; x < d.width; ++x) {
            *d.pixel(x, y) = (x + y) % 2 == 0;
        }
    }
    return d;
}

}

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
    load_into_memory<Machine::program_address>({
        0x80, 0x08,
        0x00, 0xE0, // CLS
    }, m.memory);

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
    load_into_memory<Machine::program_address>({
        0x00, 0x00,
    }, m.memory);

    auto m_expect = m;
    m_expect.fault = Fault{Fault::Type::invalid_instruction, m.program_counter};
    m_expect.program_counter += sizeof(Word);

    CHECK_FALSE(m.cycle());
    REQUIRE(m == m_expect);
}

TEST_CASE("Individual instructions execute correctly", "[machine][cycle]")
{
    SECTION("cls")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x00, 0xE0, // CLS
        }, m.memory);
        m.display = create_checkerboard();

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.display = {};

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("ret")
    {
        SECTION("with an empty stack")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x00, 0xEE, // RET
            }, m.memory);

            auto m_expect = m;
            m_expect.fault = Fault{Fault::Type::empty_stack, m.program_counter};
            m_expect.program_counter += sizeof(Word);

            CHECK_FALSE(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with a non-empty stack")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x00, 0xEE, // RET
            }, m.memory);
            m.stack.push(m.program_counter);

            auto m_expect = m;
            m_expect.stack.pop();

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("jmp_a")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x1F, 0x00, // JMP F00h
        }, m.memory);

        auto m_expect = m;
        m_expect.program_counter = 0xF00;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("call_a")
    {
        SECTION("with a full stack")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x2E, 0xEE, // CALL EEEh
            }, m.memory);
            for (std::size_t i = 0; i < m.stack.max_size(); ++i) {
                REQUIRE(m.stack.push(i));
            }

            auto m_expect = m;
            m_expect.fault = Fault{Fault::Type::full_stack, m.program_counter};
            m_expect.program_counter += sizeof(Word);

            CHECK_FALSE(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with an empty stack")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x2E, 0xEE, // CALL EEEh
            }, m.memory);

            auto m_expect = m;
            m_expect.program_counter = 0xEEE;
            m_expect.stack.push(0x202);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with an almost-full stack")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x2E, 0xEE, // CALL EEEh
            }, m.memory);
            for (std::size_t i = 0; i < m.stack.max_size() - 1; ++i) {
                REQUIRE(m.stack.push(i));
            }

            auto m_expect = m;
            m_expect.program_counter = 0xEEE;
            m_expect.stack.push(0x202);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("seq_v_b")
    {
        SECTION("when equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x3A, 0xEE, // SEQ %VA, $EEh
            }, m.memory);
            m.registers.va = 0xEE;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when not equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x3A, 0xEE, // SEQ %VA, $EEh
            }, m.memory);
            m.registers.va = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("sne_v_b")
    {
        SECTION("when not equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x4A, 0xEE, // SNE %VA, $EEh
            }, m.memory);
            m.registers.va = 0xAA;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x4A, 0xEE, // SNE %VA, $EEh
            }, m.memory);
            m.registers.va = 0xEE;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("seq_v_v")
    {
        SECTION("when equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x51, 0xE0, // SEQ %V1, %VE
            }, m.memory);
            m.registers.v1 = 0xAA;
            m.registers.ve = 0xAA;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when not equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x54, 0x00, // SEQ %V4, %V0
            }, m.memory);
            m.registers.v4 = 0xEE;
            m.registers.v0 = 0x2E;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("mov_v_b")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x6C, 0x7F, // MOV %VC, $7Fh
        }, m.memory);

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.vc = 0x7F;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("add_v_b")
    {
        SECTION("with overflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x7C, 0xFF, // ADD %VC, $FFh
            }, m.memory);
            m.registers.vc = 0x03;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vc = 0x02;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("without overflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x78, 0x20, // ADD %V8, $FFh
            }, m.memory);
            m.registers.v8 = 0x34;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v8 = 0x54;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("mov_v_v")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x8A, 0xB0, // MOV %VA, %VB
        }, m.memory);
        m.registers.va = 0x12;
        m.registers.vb = 0x36;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.va = 0x36;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("or_v_v")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x80, 0x11, // OR %V0, %V1
        }, m.memory);
        m.registers.v0 = 0b10101010;
        m.registers.v1 = 0b00011111;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.v0 = 0b10111111;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("and_v_v")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x82, 0xE2, // AND %V2, %VE
        }, m.memory);
        m.registers.v2 = 0b10101010;
        m.registers.ve = 0b00011111;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.v2 = 0b00001010;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("xor_v_v")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0x87, 0x33, // XOR %V7, %V3
        }, m.memory);
        m.registers.v7 = 0b10101010;
        m.registers.v3 = 0b00011111;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.v7 = 0b10110101;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("add_v_v")
    {
        SECTION("without overflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8A, 0xC4, // ADD %VA, %VC
            }, m.memory);
            m.registers.va = 0x0A;
            m.registers.vc = 0x75;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.va = 0x7F;
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with overflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x80, 0x14, // ADD %V0, %V1
            }, m.memory);
            m.registers.v0 = 0xFF;
            m.registers.v1 = 0x09;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v0 = 0x08;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("into %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8F, 0x04, // ADD %VF, %V0
            }, m.memory);
            m.registers.vf = 0x7F;
            m.registers.v0 = 0x21;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0xA0;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("from %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x87, 0xF4, // ADD %V7, %VF
            }, m.memory);
            m.registers.v7 = 0xA4;
            m.registers.vf = 0x4A;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v7 = 0xEE;
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("sub_v_v")
    {
        SECTION("without underflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8A, 0xC5, // SUB %VA, %VC
            }, m.memory);
            m.registers.va = 0x75;
            m.registers.vc = 0x05;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.va = 0x70;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with underflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x80, 0x15, // SUB %V0, %V1
            }, m.memory);
            m.registers.v0 = 0x00;
            m.registers.v1 = 0x01;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v0 = 0xFF;
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("into %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8F, 0x05, // SUB %VF, %V0
            }, m.memory);
            m.registers.vf = 0x7F;
            m.registers.v0 = 0x21;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x5E;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("from %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x87, 0xF5, // SUB %V7, %VF
            }, m.memory);
            m.registers.v7 = 0xA4;
            m.registers.vf = 0x4A;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v7 = 0x5A;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("shr_v")
    {
        SECTION("without lsb")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8A, 0x06, // SHR %VA
            }, m.memory);
            m.registers.va = 0x74;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.va = 0x3A;
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with lsb")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x80, 0x06, // SHR %V0
            }, m.memory);
            m.registers.v0 = 0xFF;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v0 = 0x7F;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("into %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8F, 0x06, // SHR %VF
            }, m.memory);
            m.registers.vf = 0x7F;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x3F;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("subn_v_v")
    {
        SECTION("without underflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8A, 0xC7, // SUBN %VA, %VC
            }, m.memory);
            m.registers.va = 0x05;
            m.registers.vc = 0x75;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.va = 0x70;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with underflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x80, 0x17, // SUBN %V0, %V1
            }, m.memory);
            m.registers.v0 = 0x01;
            m.registers.v1 = 0x00;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v0 = 0xFF;
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("into %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8F, 0x07, // SUBN %VF, %V0
            }, m.memory);
            m.registers.vf = 0x21;
            m.registers.v0 = 0x7F;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x5E;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("from %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x87, 0xF7, // SUBN %V7, %VF
            }, m.memory);
            m.registers.v7 = 0x4A;
            m.registers.vf = 0xA4;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v7 = 0x5A;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("shl_v")
    {
        SECTION("without msb")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8A, 0x0E, // SHL %VA
            }, m.memory);
            m.registers.va = 0b01111111;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.va = 0b11111110;
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with msb")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x80, 0x0E, // SHL %V0
            }, m.memory);
            m.registers.v0 = 0b11111111;
            m.registers.vf = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v0 = 0b11111110;
            m_expect.registers.vf = 0x01;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("into %VF")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x8F, 0x0E, // SHL %VF
            }, m.memory);
            m.registers.vf = 0b01111111;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0b11111110;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("sne_v_v")
    {
        SECTION("when not equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x9A, 0xE0, // SNE %VA, %VE
            }, m.memory);
            m.registers.va = 0xAA;
            m.registers.ve = 0x11;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when equal")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0x9A, 0xE0, // SNE %VA, %VE
            }, m.memory);
            m.registers.va = 0xEE;
            m.registers.ve = 0xEE;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("mov_i_a")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0xAE, 0xEE, // MOV %I, $EEEh
        }, m.memory);

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.i = 0xEEE;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("jmp_v0_a")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0xBA, 0xAA, // JMP AAAh(%V0)
        }, m.memory);
        m.registers.v0 = 0x22;

        auto m_expect = m;
        m_expect.program_counter = 0xACC;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("rnd_v_b")
    {
        SECTION("with an empty mask")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xCA, 0x00, // RND %VA, $00h
            }, m.memory);

            auto const pc_expect = m.program_counter + sizeof(Word);

            CHECK(m.cycle());

            auto m_expect = m;
            m_expect.program_counter = pc_expect;
            REQUIRE(m == m_expect);
            REQUIRE((m.registers.va & ~0x00) == 0x00);
        }

        SECTION("with with a partial mask")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xCA, 0xA5, // RND %VA, $A5h
            }, m.memory);

            auto const pc_expect = m.program_counter + sizeof(Word);

            CHECK(m.cycle());

            auto m_expect = m;
            m_expect.program_counter = pc_expect;
            REQUIRE(m == m_expect);
            REQUIRE((m.registers.va & ~0xA5) == 0x00);
        }

        SECTION("with with a full mask")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xCA, 0xFF, // RND %VA, $FFh
            }, m.memory);

            auto const pc_expect = m.program_counter + sizeof(Word);

            CHECK(m.cycle());

            auto m_expect = m;
            m_expect.program_counter = pc_expect;
            REQUIRE(m == m_expect);
            REQUIRE((m.registers.va & ~0xFF) == 0x00);
        }
    }

    SECTION("drw_v_v_n")
    {
        SECTION("with zero rows")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xD0, 0x10, // DRW %V0, %V1, $0h
            }, m.memory);
            m.registers.v0 = 0x00;
            m.registers.v1 = 0x00;
            m.registers.vf = 0xFF;
            m.registers.i = 0x300;
            m.memory[0x300] = 0b10101010;
            m.display = create_checkerboard();

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with one row")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xD1, 0x21, // DRW %V1, %V2, $1h
            }, m.memory);
            m.registers.v1 = 0x01;
            m.registers.v2 = 0x02;
            m.registers.vf = 0xFF;
            m.registers.i = 0x300;
            m.memory[0x300] = 0b10100111;
            *m.display.pixel(0, 0) = true;
            *m.display.pixel(0, 2) = true;
            *m.display.pixel(1, 2) = true;
            *m.display.pixel(2, 2) = true;
            *m.display.pixel(3, 2) = true;
            *m.display.pixel(4, 2) = true;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x01;
            *m_expect.display.pixel(1, 2) = false;
            *m_expect.display.pixel(3, 2) = false;
            *m_expect.display.pixel(6, 2) = true;
            *m_expect.display.pixel(7, 2) = true;
            *m_expect.display.pixel(8, 2) = true;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("without clearing pixels")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xD0, 0x11, // DRW %V0, %V1, $1h
            }, m.memory);
            m.registers.v0 = 0x00;
            m.registers.v1 = 0x00;
            m.registers.vf = 0xFF;
            m.registers.i = 0x300;
            m.memory[0x300] = 0b00000000;
            m.display = create_checkerboard();

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("outside of bounds")
        {
            // Draw an 8x15 sprite at the bottom-right corner of the screen
            // such that only one-quarter of the sprite is visible.
            Machine m;
            load_into_memory<Machine::program_address>({
                0xD0, 0x1F, // DRW %V0, %V1, $Fh
            }, m.memory);
            m.registers.v0 = 0x3C;
            m.registers.v1 = 0x18;
            m.registers.vf = 0xFF;
            m.registers.i = 0x300;
            m.memory[0x300] = 0b11001111;
            m.memory[0x301] = 0b01101111;
            m.memory[0x302] = 0b00111111;
            m.memory[0x303] = 0b00011111;
            m.memory[0x304] = 0b00001111;
            m.memory[0x305] = 0b00011111;
            m.memory[0x306] = 0b00111111;
            m.memory[0x307] = 0b01101111;
            m.memory[0x308] = 0b11001111;
            m.memory[0x309] = 0b11111111;
            m.memory[0x30A] = 0b11111111;
            m.memory[0x30B] = 0b11111111;
            m.memory[0x30C] = 0b11111111;
            m.memory[0x30D] = 0b11111111;
            m.memory[0x30E] = 0b11111111;
            m.memory[0x30F] = 0b11111111;
            m.display = create_checkerboard();

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x01;
            *m_expect.display.pixel(60, 24) = false;
            *m_expect.display.pixel(61, 24) = true;
            *m_expect.display.pixel(62, 24) = true;
            *m_expect.display.pixel(63, 24) = false;
            *m_expect.display.pixel(60, 25) = false;
            *m_expect.display.pixel(61, 25) = false;
            *m_expect.display.pixel(62, 25) = true;
            *m_expect.display.pixel(63, 25) = true;
            *m_expect.display.pixel(60, 26) = true;
            *m_expect.display.pixel(61, 26) = false;
            *m_expect.display.pixel(62, 26) = false;
            *m_expect.display.pixel(63, 26) = true;
            *m_expect.display.pixel(60, 27) = false;
            *m_expect.display.pixel(61, 27) = true;
            *m_expect.display.pixel(62, 27) = false;
            *m_expect.display.pixel(63, 27) = false;
            *m_expect.display.pixel(60, 28) = true;
            *m_expect.display.pixel(61, 28) = false;
            *m_expect.display.pixel(62, 28) = true;
            *m_expect.display.pixel(63, 28) = false;
            *m_expect.display.pixel(60, 29) = false;
            *m_expect.display.pixel(61, 29) = true;
            *m_expect.display.pixel(62, 29) = false;
            *m_expect.display.pixel(63, 29) = false;
            *m_expect.display.pixel(60, 30) = true;
            *m_expect.display.pixel(61, 30) = false;
            *m_expect.display.pixel(62, 30) = false;
            *m_expect.display.pixel(63, 30) = true;
            *m_expect.display.pixel(60, 31) = false;
            *m_expect.display.pixel(61, 31) = false;
            *m_expect.display.pixel(62, 31) = true;
            *m_expect.display.pixel(63, 31) = true;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("skp_v")
    {
        SECTION("when only that key is pressed")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE0, 0x9E, // SKP %V0
            }, m.memory);
            m.registers.v0 = 0x00;
            m.keys.set(to_index(Key::k0));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when pressed among other keys")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xEA, 0x9E, // SKP %VA
            }, m.memory);
            m.registers.va = 0x0F;
            m.keys.set(to_index(Key::k0));
            m.keys.set(to_index(Key::kf));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when only other keys are pressed")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE1, 0x9E, // SKP %V1
            }, m.memory);
            m.registers.v1 = 0x0A;
            m.keys.set(to_index(Key::kb));
            m.keys.set(to_index(Key::ke));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when no key is pressed")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE2, 0x9E, // SKP %V2
            }, m.memory);
            m.registers.v2 = 0x0C;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when the key is out of bounds")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE3, 0x9E, // SKP %V3
            }, m.memory);
            m.registers.v3 = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("sknp_v")
    {
        SECTION("when only that key is pressed")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE0, 0xA1, // SKNP %V0
            }, m.memory);
            m.registers.v0 = 0x00;
            m.keys.set(to_index(Key::k0));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when pressed among other keys")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xEA, 0xA1, // SKNP %VA
            }, m.memory);
            m.registers.va = 0x0F;
            m.keys.set(to_index(Key::k0));
            m.keys.set(to_index(Key::kf));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when only other keys are pressed")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE1, 0xA1, // SKNP %V1
            }, m.memory);
            m.registers.v1 = 0x0A;
            m.keys.set(to_index(Key::kb));
            m.keys.set(to_index(Key::ke));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when no key is pressed")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE2, 0xA1, // SKNP %V2
            }, m.memory);
            m.registers.v2 = 0x0C;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when the key is out of bounds")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xE3, 0xA1, // SKNP %V3
            }, m.memory);
            m.registers.v3 = 0xFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("mov_v_dt")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0xFC, 0x07, // MOV %VC, %DT
        }, m.memory);
        m.registers.vc = 0xFF;
        m.registers.dt = 0xAC;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.vc = 0xAC;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("wkp_v")
    {
        SECTION("when no key is pressed within 100 cycles")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xF1, 0x0A, // WKP %V1
            }, m.memory);
            m.registers.v1 = 0xFF;

            auto m_expect = m;

            for (auto i = 0u; i < 100; ++i) {
                CHECK(m.cycle());
                REQUIRE(m == m_expect);
            }
        }

        SECTION("when one key is pressed on the second cycle")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xF2, 0x0A, // WKP %V2
            }, m.memory);
            m.registers.v2 = 0xFF;

            auto m_expect = m;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);

            m.keys.set(to_index(Key::ka));

            m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.v2 = 0x0A;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when one key is pressed on the first cycle")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xFA, 0x0A, // WKP %VA
            }, m.memory);
            m.registers.va = 0xFF;
            m.keys.set(to_index(Key::k4));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.va = 0x04;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when multiple keys are pressed on the first cycle")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xFF, 0x0A, // WKP %VF
            }, m.memory);
            m.registers.vf = 0xFF;
            m.keys.set(to_index(Key::k0));
            m.keys.set(to_index(Key::kf));

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.vf = 0x00;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    SECTION("mov_dt_v")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0xFD, 0x15, // MOV %DT, %VD
        }, m.memory);
        m.registers.vd = 0xCD;
        m.registers.dt = 0xFF;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.dt = 0xCD;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("mov_st_v")
    {
        Machine m;
        load_into_memory<Machine::program_address>({
            0xF7, 0x18, // MOV %ST, %V7
        }, m.memory);
        m.registers.v7 = 0x77;
        m.registers.st = 0xFF;

        auto m_expect = m;
        m_expect.program_counter += sizeof(Word);
        m_expect.registers.st = 0x77;

        CHECK(m.cycle());
        REQUIRE(m == m_expect);
    }

    SECTION("add_i_v")
    {
        SECTION("without overflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xF5, 0x1E, // ADD %I, %V5
            }, m.memory);
            m.registers.v5 = 0xAC;
            m.registers.i = 0xDEA;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.i = 0xE96;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with overflow")
        {
            Machine m;
            load_into_memory<Machine::program_address>({
                0xF0, 0x1E, // ADD %I, %V0
            }, m.memory);
            m.registers.v0 = 0x02;
            m.registers.i = 0xFFF;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);
            m_expect.registers.i = 0x001;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

    // TODO: font_v = 0xF029,
    // TODO: bcd_v = 0xF033,
    // TODO: mov_ii_v = 0xF055,
    // TODO: mov_v_ii = 0xF065,
}
