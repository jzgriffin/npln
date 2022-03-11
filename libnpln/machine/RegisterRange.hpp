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

#ifndef LIBNPLN_MACHINE_REGISTERRANGE_HPP
#define LIBNPLN_MACHINE_REGISTERRANGE_HPP

#include <libnpln/machine/Register.hpp>

#include <iterator>
#include <optional>
#include <stdexcept>

namespace libnpln::machine {

class RegisterIterator
{
public:
    using difference_type = std::ptrdiff_t;
    using value_type = Register;
    using pointer = Register const*;
    using reference = Register const&;
    using iterator_category = std::input_iterator_tag;

    RegisterIterator() : RegisterIterator{std::nullopt} {}

    explicit RegisterIterator(std::optional<Register> r) : current{r} {}

    constexpr auto operator==(RegisterIterator const& rhs) const noexcept
    {
        return current == rhs.current;
    }

    constexpr auto operator!=(RegisterIterator const& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    auto operator++() -> RegisterIterator&
    {
        if (current == std::nullopt) {
            return *this;
        }

        current = next(*current);
        return *this;
    }

    auto operator++(int) -> RegisterIterator
    {
        auto i = *this;
        ++(*this);
        return i;
    }

    auto operator*() const -> reference
    {
        return *current;
    }

    auto operator->() const -> pointer
    {
        return &*current;
    }

    static constexpr auto next(Register const r) -> std::optional<Register>
    {
        switch (r) {
        case Register::v0: return Register::v1;
        case Register::v1: return Register::v2;
        case Register::v2: return Register::v3;
        case Register::v3: return Register::v4;
        case Register::v4: return Register::v5;
        case Register::v5: return Register::v6;
        case Register::v6: return Register::v7;
        case Register::v7: return Register::v8;
        case Register::v8: return Register::v9;
        case Register::v9: return Register::va;
        case Register::va: return Register::vb;
        case Register::vb: return Register::vc;
        case Register::vc: return Register::vd;
        case Register::vd: return Register::ve;
        case Register::ve: return Register::vf;
        case Register::vf: return std::nullopt;
        }

        throw std::out_of_range("Unknown Register in RegisterIterator::next");
    }

private:
    std::optional<Register> current;
};

class RegisterRange
{
public:
    RegisterRange() : RegisterRange{Register::vf} {}

    explicit RegisterRange(Register const last) : RegisterRange{Register::v0, last} {}

    RegisterRange(Register const first, Register const last)
        : first{first}, last{RegisterIterator::next(last)}
    {}

    [[nodiscard]] auto begin() const noexcept
    {
        return first;
    }
    [[nodiscard]] auto end() const noexcept
    {
        return last;
    }

private:
    RegisterIterator first;
    RegisterIterator last;
};

} // namespace libnpln::machine

#endif
