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

#include <libnpln/machine/Memory.hpp>

#include <catch2/catch.hpp>

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>

using namespace libnpln::machine;
using namespace std::string_literals;

TEST_CASE("Memory stores 4096 bytes", "[machine][memory]")
{
    static_assert(std::is_same_v<Memory::value_type, Byte>);
    REQUIRE(Memory{}.max_size() == 4096);
}

TEST_CASE("Memory can be loaded from a stream", "[machine][memory]")
{
    auto const str = "test"s;
    auto s = std::istringstream{str};
    auto m = Memory{};
    auto const a = Address{4};
    REQUIRE(load_into_memory(s, m, a));
    REQUIRE(std::memcmp(str.data(), m.data() + a, str.size() + 1) == 0);
}

TEST_CASE("Memory cannot be loaded from a stream larger than the memory",
    "[machine][memory]")
{
    auto const str = "some test data"s;
    auto s = std::istringstream{str};
    auto m = Memory{};
    auto const a = static_cast<Address>(m.size() - str.size() + 1);
    REQUIRE_FALSE(load_into_memory(s, m, a));
}

TEST_CASE("Memory can be loaded from a file", "[machine][memory]")
{
    auto const str = "some test data from a file"s;
    auto const p = std::filesystem::path{"machine-memory-test-file"};

    {
        auto s = std::ofstream{p, std::ios::trunc | std::ios::binary};
        REQUIRE(s.write(str.data(), str.size() + 1));
    }

    auto m = Memory{};
    auto const a = Address{4};
    REQUIRE(load_into_memory(p, m, a));
    REQUIRE(std::memcmp(str.data(), m.data() + a, str.size() + 1) == 0);
}

TEST_CASE("Memory cannot be loaded from a file larger than the memory",
    "[machine][memory]")
{
    auto const str = "some test data from a file"s;
    auto const p = std::filesystem::path{"machine-memory-test-file"};

    {
        auto s = std::ofstream{p, std::ios::trunc | std::ios::binary};
        REQUIRE(s.write(str.data(), str.size() + 1));
    }

    auto m = Memory{};
    auto const a = static_cast<Address>(m.size() - str.size() + 1);
    REQUIRE_FALSE(load_into_memory(p, m, a));
}

TEST_CASE("Programs can be loaded from a stream", "[machine][memory]")
{
    auto const str = "test"s;
    auto s = std::istringstream{str};
    auto const m = load_program(s);
    REQUIRE(m != std::nullopt);
    REQUIRE(std::memcmp(str.data(), m->data() + program_address,
        str.size() + 1) == 0);
}

TEST_CASE("Programs cannot be loaded from a stream larger than the memory",
    "[machine][memory]")
{
    auto const str = std::string(Memory{}.size() - program_address, '0');
    auto s = std::istringstream{str};
    auto const m = load_program(s);
    REQUIRE(m == std::nullopt);
}

TEST_CASE("Programs can be loaded from a file", "[machine][memory]")
{
    auto const str = "some test data from a file"s;
    auto const p = std::filesystem::path{"machine-memory-test-file"};

    {
        auto s = std::ofstream{p, std::ios::trunc | std::ios::binary};
        REQUIRE(s.write(str.data(), str.size() + 1));
    }

    auto const m = load_program(p);
    REQUIRE(m != std::nullopt);
    REQUIRE(std::memcmp(str.data(), m->data() + program_address,
        str.size() + 1) == 0);
}

TEST_CASE("Programs cannot be loaded from a file larger than the memory",
    "[machine][memory]")
{
    auto const str = std::string(Memory{}.size() - program_address - 1, '0');
    auto const p = std::filesystem::path{"machine-memory-test-file"};

    {
        auto s = std::ofstream{p, std::ios::trunc | std::ios::binary};
        REQUIRE(s.write(str.data(), str.size() + 1));
    }

    auto const m = load_program(p);
    REQUIRE(m == std::nullopt);
}
