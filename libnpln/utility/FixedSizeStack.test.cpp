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

#include <libnpln/utility/FixedSizeStack.hpp>

#include <catch2/catch.hpp>

#include <iterator>
#include <numeric>
#include <vector>

using namespace libnpln::utility;

SCENARIO("FixedSizeStack can be compared", "[utility][fixedsizestack]")
{
    GIVEN("a stack with some elements")
    {
        auto s1 = FixedSizeStack<int, 5>{};
        s1.push(4);
        s1.push(7);
        s1.push(3);

        WHEN("it is compared with itself")
        {
            THEN("it is equal")
            {
                REQUIRE(s1 == s1);
            }
        }

        AND_GIVEN("a stack with different elements")
        {
            auto s2 = decltype(s1){};
            s2.push(8);
            s2.push(3);

            WHEN("they are compared with each other")
            {
                THEN("they are not equal")
                {
                    REQUIRE(s1 != s2);
                    REQUIRE(s2 != s1);
                }
            }
        }
    }
}

SCENARIO("FixedSizeStack can be iterated over", "[utility][fixedsizestack]")
{
    GIVEN("a vector with some elements")
    {
        auto const v = std::vector<int>{8, 1, 3, 7};

        AND_GIVEN("a stack with the same elements")
        {
            auto s = FixedSizeStack<int, 10>{};
            for (auto&& x : v) {
                s.push(x);
            }

            WHEN("the vector and stack are compared with std::equal")
            {
                THEN("they are equal")
                {
                    REQUIRE(std::equal(std::begin(v), std::end(v),
                        std::begin(s), std::end(s)));
                }
            }
        }
    }
}

SCENARIO("FixedSizeStack respects its size constraint",
    "[utility][fixedsizestack]")
{
    GIVEN("an empty stack")
    {
        auto s = FixedSizeStack<bool, 10>{};
        REQUIRE(s.empty());
        REQUIRE_FALSE(s.full());

        WHEN("it is pushed onto")
        {
            REQUIRE(s.push(false));

            THEN("it is no longer empty")
            {
                REQUIRE_FALSE(s.empty());
                REQUIRE_FALSE(s.full());

                AND_WHEN("it is popped off of")
                {
                    REQUIRE(s.pop() != std::nullopt);

                    THEN("it is empty again")
                    {
                        REQUIRE(s.empty());
                        REQUIRE_FALSE(s.full());
                    }
                }
            }
        }

        WHEN("it is filled")
        {
            for (auto i = 0U; i < decltype(s)::max_size(); ++i) {
                REQUIRE(s.push(false));
            }

            THEN("it is full")
            {
                REQUIRE_FALSE(s.empty());
                REQUIRE(s.full());

                AND_WHEN("it is popped off of")
                {
                    REQUIRE(s.pop() != std::nullopt);

                    THEN("it is no longer full")
                    {
                        REQUIRE_FALSE(s.empty());
                        REQUIRE_FALSE(s.full());
                    }
                }
            }
        }
    }
}

SCENARIO("FixedSizeStack implements push and pop",
    "[utility][fixedsizestack]")
{
    GIVEN("an empty stack")
    {
        auto s = FixedSizeStack<int, 100>{};
        REQUIRE(s.empty());

        WHEN("it is popped off of")
        {
            auto const x = s.pop();

            THEN("the popped element is null")
            {
                REQUIRE(x == std::nullopt);
            }
        }

        WHEN("it is pushed onto")
        {
            auto const x = 1;
            REQUIRE(s.push(x));

            THEN("the top element is the pushed element")
            {
                REQUIRE(s.top() == x);

                AND_WHEN("it is pushed onto again")
                {
                    auto const y = 2;
                    REQUIRE(s.push(y));

                    THEN("the top element is the pushed element")
                    {
                        REQUIRE(s.top() == y);

                        AND_WHEN("it is popped off of")
                        {
                            auto const z = s.pop();

                            THEN("the popped element is the pushed element")
                            {
                                REQUIRE(z == y);
                            }
                        }
                    }
                }

                AND_WHEN("it is popped off of")
                {
                    auto const y = s.pop();

                    THEN("the popped element is the pushed element")
                    {
                        REQUIRE(y == x);
                    }
                }
            }
        }

        WHEN("it is filled")
        {
            auto xs = std::vector<decltype(s)::value_type>{};
            xs.resize(decltype(s)::max_size());
            std::iota(std::begin(xs), std::end(xs), 0);

            for (auto&& x : xs) {
                REQUIRE(s.push(x));
            }

            THEN("it cannot be pushed onto")
            {
                REQUIRE_FALSE(s.push(0));

                AND_WHEN("all of its elements are popped off")
                {
                    auto ys = decltype(xs){};
                    for (auto y = s.pop(); y != std::nullopt; y = s.pop()) {
                        ys.push_back(y.value());
                    }

                    THEN("the elements are in reverse order")
                    {
                        std::reverse(std::begin(ys), std::end(ys));
                        REQUIRE(xs == ys);
                    }
                }
            }

            THEN("it cannot be emplaced onto")
            {
                REQUIRE_FALSE(s.emplace(0));
            }
        }
    }

    GIVEN("an stack of pairs")
    {
        auto s = FixedSizeStack<std::pair<int, int>, 10>{};

        WHEN("it is emplaced onto")
        {
            auto const x = 4;
            auto const y = 7;
            REQUIRE(s.emplace(x, y));

            THEN("the top element is the emplaced pair")
            {
                REQUIRE(s.top() == std::make_pair(x, y));
            }
        }
    }
}
