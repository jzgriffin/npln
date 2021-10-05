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

#include <libnpln/machine/Display.hpp>

#include <catch2/catch.hpp>

using namespace libnpln::machine;

SCENARIO("Display pixels can be get and set", "[machine][display]")
{
    GIVEN("An empty display")
    {
        auto d = Display{};
        auto const& d_const = d;

        WHEN("it is initialized")
        {
            THEN("every pixel is unset")
            {
                for (auto x = 0U; x < decltype(d)::width; x++) {
                    for (auto y = 0U; y < decltype(d)::height; y++) {
                        auto* p = d.pixel(x, y);
                        REQUIRE(p);
                        REQUIRE_FALSE(*p);
                    }
                }
            }
        }

        WHEN("the top-left pixel is set")
        {
            auto const x = 0;
            auto const y = 0;
            auto* p = d.pixel(x, y);
            REQUIRE(p);
            *p = true;

            THEN("its value can be read back")
            {
                auto const* p_const = d_const.pixel(x, y);
                REQUIRE(p_const);
                REQUIRE(*p);
            }
        }

        WHEN("the top-right pixel is set")
        {
            auto const x = decltype(d)::width - 1;
            auto const y = 0;
            auto* p = d.pixel(x, y);
            REQUIRE(p);
            *p = true;

            THEN("its value can be read back")
            {
                auto const* p_const = d_const.pixel(x, y);
                REQUIRE(p_const);
                REQUIRE(*p);
            }
        }

        WHEN("the bottom-left pixel is set")
        {
            auto const x = 0;
            auto const y = decltype(d)::height - 1;
            auto* p = d.pixel(x, y);
            REQUIRE(p);
            *p = true;

            THEN("its value can be read back")
            {
                auto const* p_const = d_const.pixel(x, y);
                REQUIRE(p_const);
                REQUIRE(*p);
            }
        }

        WHEN("the bottom-right pixel is set")
        {
            auto const x = decltype(d)::width - 1;
            auto const y = decltype(d)::height - 1;
            auto* p = d.pixel(x, y);
            REQUIRE(p);
            *p = true;

            THEN("its value can be read back")
            {
                auto const* p_const = d_const.pixel(x, y);
                REQUIRE(p_const);
                REQUIRE(*p);
            }
        }

        WHEN("an out-of-range pixel is indexed")
        {
            THEN("its proxy is invalid")
            {
                REQUIRE_FALSE(d.pixel(decltype(d)::width, 0));
                REQUIRE_FALSE(d.pixel(0, decltype(d)::height));
                REQUIRE_FALSE(d.pixel(decltype(d)::width, decltype(d)::height));
            }
        }
    }
}

SCENARIO("Display can be cleared", "[machine][display]")
{
    GIVEN("A solid display")
    {
        auto d = Display{};
        for (auto x = 0U; x < decltype(d)::width; x++) {
            for (auto y = 0U; y < decltype(d)::height; y++) {
                auto* p = d.pixel(x, y);
                REQUIRE(p);
                *p = true;
            }
        }

        WHEN("it is cleared")
        {
            d.clear();

            THEN("every pixel is unset")
            {
                for (auto x = 0U; x < decltype(d)::width; x++) {
                    for (auto y = 0U; y < decltype(d)::height; y++) {
                        auto* p = d.pixel(x, y);
                        REQUIRE(p);
                        REQUIRE_FALSE(*p);
                    }
                }
            }
        }
    }
}
