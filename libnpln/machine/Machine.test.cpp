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
    SECTION("cls")
    {
        Machine m;
        m.memory = create_program({
            0x00, 0xE0, // CLS
        });

        // Checkerboard pattern
        for (std::size_t y = 0; y < m.display.height; ++y) {
            for (std::size_t x = 0; x < m.display.width; ++x) {
                *m.display.pixel(x, y) = (x + y) % 2 == 0;
            }
        }

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
            m.memory = create_program({
                0x00, 0xEE, // RET
            });

            auto m_expect = m;
            m_expect.fault = Fault{Fault::Type::empty_stack, m.program_counter};
            m_expect.program_counter += sizeof(Word);

            CHECK_FALSE(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with a non-empty stack")
        {
            Machine m;
            m.memory = create_program({
                0x00, 0xEE, // RET
            });
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
        m.memory = create_program({
            0x1F, 0x00, // JMP F00h
        });

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
            m.memory = create_program({
                0x2E, 0xEE, // CALL EEEh
            });
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
            m.memory = create_program({
                0x2E, 0xEE, // CALL EEEh
            });

            auto m_expect = m;
            m_expect.program_counter = 0xEEE;
            m_expect.stack.push(0x202);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("with an almost-full stack")
        {
            Machine m;
            m.memory = create_program({
                0x2E, 0xEE, // CALL EEEh
            });
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
            m.memory = create_program({
                0x3A, 0xEE, // SEQ %VA, $EEh
            });
            m.registers.va = 0xEE;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when not equal")
        {
            Machine m;
            m.memory = create_program({
                0x3A, 0xEE, // SEQ %VA, $EEh
            });
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
            m.memory = create_program({
                0x4A, 0xEE, // SNE %VA, $EEh
            });
            m.registers.va = 0xAA;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word) * 2;

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }

        SECTION("when equal")
        {
            Machine m;
            m.memory = create_program({
                0x4A, 0xEE, // SNE %VA, $EEh
            });
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
            m.memory = create_program({
                0x51, 0xE0, // SEQ %V1, %VE
            });
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
            m.memory = create_program({
                0x54, 0x00, // SEQ %V4, %V0
            });
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
        m.memory = create_program({
            0x6C, 0x7F, // MOV %VC, $7Fh
        });

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
            m.memory = create_program({
                0x7C, 0xFF, // ADD %VC, $FFh
            });
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
            m.memory = create_program({
                0x78, 0x20, // ADD %V8, $FFh
            });
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
        m.memory = create_program({
            0x8A, 0xB0, // MOV %VA, %VB
        });
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
        m.memory = create_program({
            0x80, 0x11, // OR %V0, %V1
        });
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
        m.memory = create_program({
            0x82, 0xE2, // AND %V2, %VE
        });
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
        m.memory = create_program({
            0x87, 0x33, // XOR %V7, %V3
        });
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
            m.memory = create_program({
                0x8A, 0xC4, // ADD %VA, %VC
            });
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
            m.memory = create_program({
                0x80, 0x14, // ADD %V0, %V1
            });
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
            m.memory = create_program({
                0x8F, 0x04, // ADD %VF, %V0
            });
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
            m.memory = create_program({
                0x87, 0xF4, // ADD %V7, %VF
            });
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
            m.memory = create_program({
                0x8A, 0xC5, // SUB %VA, %VC
            });
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
            m.memory = create_program({
                0x80, 0x15, // SUB %V0, %V1
            });
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
            m.memory = create_program({
                0x8F, 0x05, // SUB %VF, %V0
            });
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
            m.memory = create_program({
                0x87, 0xF5, // SUB %V7, %VF
            });
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
            m.memory = create_program({
                0x8A, 0x06, // SHR %VA
            });
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
            m.memory = create_program({
                0x80, 0x06, // SHR %V0
            });
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
            m.memory = create_program({
                0x8F, 0x06, // SHR %VF
            });
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
            m.memory = create_program({
                0x8A, 0xC7, // SUBN %VA, %VC
            });
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
            m.memory = create_program({
                0x80, 0x17, // SUBN %V0, %V1
            });
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
            m.memory = create_program({
                0x8F, 0x07, // SUBN %VF, %V0
            });
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
            m.memory = create_program({
                0x87, 0xF7, // SUBN %V7, %VF
            });
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
            m.memory = create_program({
                0x8A, 0x0E, // SHL %VA
            });
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
            m.memory = create_program({
                0x80, 0x0E, // SHL %V0
            });
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
            m.memory = create_program({
                0x8F, 0x0E, // SHL %VF
            });
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
            m.memory = create_program({
                0x9A, 0xE0, // SNE %VA, %VE
            });
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
            m.memory = create_program({
                0x9A, 0xE0, // SNE %VA, %VE
            });
            m.registers.va = 0xEE;
            m.registers.ve = 0xEE;

            auto m_expect = m;
            m_expect.program_counter += sizeof(Word);

            CHECK(m.cycle());
            REQUIRE(m == m_expect);
        }
    }

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
