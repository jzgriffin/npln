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

#include <libnpln/machine/Key.hpp>
#include <libnpln/machine/Keys.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

TEST_CASE("Keys has as many bits as there are keys", "[machine][keys]")
{
    REQUIRE(Keys{}.size() == key_count);
}

SCENARIO("The keys difference algorithm detects changes", "[machine][keys]")
{
    GIVEN("A key set")
    {
        auto ks0 = Keys{};
        ks0.set(to_index(Key::k0));
        ks0.set(to_index(Key::k3));
        ks0.set(to_index(Key::kf));

        WHEN("no keys are changed")
        {
            auto const ks1 = ks0;

            THEN("the key set differences are empty")
            {
                auto const [pks, rks] = keys_difference(ks0, ks1);
                REQUIRE(pks.empty());
                REQUIRE(rks.empty());
            }
        }

        WHEN("some keys are set")
        {
            auto pressed_keys = std::set<Key>{Key::k1, Key::k2};
            auto ks1 = ks0;
            for (auto&& i : pressed_keys) {
                ks1.set(to_index(i));
            }

            THEN("the key set differences include the changed keys")
            {
                auto const [pks, rks] = keys_difference(ks0, ks1);
                REQUIRE(pks == pressed_keys);
                REQUIRE(rks.empty());
            }
        }

        WHEN("some keys are reset")
        {
            auto released_keys = std::set<Key>{Key::k3, Key::kf};
            auto ks1 = ks0;
            for (auto&& i : released_keys) {
                ks1.reset(to_index(i));
            }

            THEN("the key set differences include the changed keys")
            {
                auto const [pks, rks] = keys_difference(ks0, ks1);
                REQUIRE(pks.empty());
                REQUIRE(rks == released_keys);
            }
        }

        WHEN("some keys are set and others are unset")
        {
            auto pressed_keys = std::set<Key>{Key::k2};
            auto released_keys = std::set<Key>{Key::k0};
            auto ks1 = ks0;
            for (auto&& i : pressed_keys) {
                ks1.set(to_index(i));
            }
            for (auto&& i : released_keys) {
                ks1.reset(to_index(i));
            }

            THEN("the key set differences include the changed keys")
            {
                auto const [pks, rks] = keys_difference(ks0, ks1);
                REQUIRE(pks == pressed_keys);
                REQUIRE(rks == released_keys);
            }
        }
    }
}
