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

#include <libnpln/utility/BitSetDifference.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::utility;

SCENARIO("The bit set difference algorithm detects changes", "[utility][bitsetdifference]")
{
    GIVEN("A bit set")
    {
        auto const bs0 = std::bitset<4>{0b1001};

        WHEN("no bits are changed")
        {
            auto const bs1 = bs0;

            THEN("the bit set difference is empty")
            {
                REQUIRE(bit_set_difference(bs0, bs1).empty());
            }
        }

        WHEN("some bits are set")
        {
            auto set_bits = std::set<std::size_t>{1, 2};
            auto bs1 = bs0;
            for (auto&& i : set_bits) {
                bs1.set(i);
            }

            THEN("the bit set difference includes the changed bits")
            {
                REQUIRE(bit_set_difference(bs0, bs1) == set_bits);
            }
        }

        WHEN("some bits are reset")
        {
            auto reset_bits = std::set<std::size_t>{3};
            auto bs1 = bs0;
            for (auto&& i : reset_bits) {
                bs1.reset(i);
            }

            THEN("the bit set difference includes the changed bits")
            {
                REQUIRE(bit_set_difference(bs0, bs1) == reset_bits);
            }
        }

        WHEN("some bits are set and others are unset")
        {
            auto set_bits = std::set<std::size_t>{2};
            auto reset_bits = std::set<std::size_t>{0};
            auto bs1 = bs0;
            for (auto&& i : set_bits) {
                bs1.set(i);
            }
            for (auto&& i : reset_bits) {
                bs1.reset(i);
            }

            auto changed_bits = set_bits;
            changed_bits.insert(std::begin(reset_bits), std::end(reset_bits));

            THEN("the bit set difference includes the changed bits")
            {
                REQUIRE(bit_set_difference(bs0, bs1) == changed_bits);
            }
        }
    }
}

SCENARIO("The bit set split difference algorithm detects changes", "[utility][bitsetdifference]")
{
    GIVEN("A bit set")
    {
        auto const bs0 = std::bitset<4>(0b1001);

        WHEN("no bits are changed")
        {
            auto const bs1 = bs0;

            THEN("the bit set differences are empty")
            {
                auto const [sbs, rbs] = bit_set_split_difference(bs0, bs1);
                REQUIRE(sbs.empty());
                REQUIRE(rbs.empty());
            }
        }

        WHEN("some bits are set")
        {
            auto set_bits = std::set<std::size_t>{1, 2};
            auto bs1 = bs0;
            for (auto&& i : set_bits) {
                bs1.set(i);
            }

            THEN("the bit set differences include the changed bits")
            {
                auto const [sbs, rbs] = bit_set_split_difference(bs0, bs1);
                REQUIRE(sbs == set_bits);
                REQUIRE(rbs.empty());
            }
        }

        WHEN("some bits are reset")
        {
            auto reset_bits = std::set<std::size_t>{3};
            auto bs1 = bs0;
            for (auto&& i : reset_bits) {
                bs1.reset(i);
            }

            THEN("the bit set differences include the changed bits")
            {
                auto const [sbs, rbs] = bit_set_split_difference(bs0, bs1);
                REQUIRE(sbs.empty());
                REQUIRE(rbs == reset_bits);
            }
        }

        WHEN("some bits are set and others are unset")
        {
            auto set_bits = std::set<std::size_t>{2};
            auto reset_bits = std::set<std::size_t>{0};
            auto bs1 = bs0;
            for (auto&& i : set_bits) {
                bs1.set(i);
            }
            for (auto&& i : reset_bits) {
                bs1.reset(i);
            }

            THEN("the bit set differences include the changed bits")
            {
                auto const [sbs, rbs] = bit_set_split_difference(bs0, bs1);
                REQUIRE(sbs == set_bits);
                REQUIRE(rbs == reset_bits);
            }
        }
    }
}
