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

#ifndef LIBNPLN_MACHINE_OPERANDS_HPP
#define LIBNPLN_MACHINE_OPERANDS_HPP

#include <libnpln/machine/DataUnits.hpp>
#include <libnpln/machine/Operand.hpp>
#include <libnpln/machine/Register.hpp>

#include <fmt/format.h>

#include <utility>
#include <variant>

namespace libnpln::machine {

namespace detail {

    template<typename T>
    struct BaseOperands
    {
        template<typename... TTypes>
        struct WithTypes
        {
            template<typename... TCodecs>
            struct WithCodecs
            {
                static constexpr auto decode(Word const w) noexcept
                {
                    (void)w; // To suppress warning in nullary cases.
                    return T{(static_cast<TTypes>(TCodecs::decode(w)))...};
                }

            protected:
                static constexpr auto encode(TTypes... xs) noexcept
                {
                    return static_cast<Word>((0x0000 | ...
                        | TCodecs::encode(static_cast<typename TCodecs::Container>(xs))));
                }

            private:
                static_assert(sizeof...(TTypes) == sizeof...(TCodecs),
                    "Mismatched number of operand and codec types");
            };
        };
    };

} // namespace detail

struct NullaryOperands : detail::BaseOperands<NullaryOperands>::WithTypes<>::WithCodecs<>
{
    constexpr auto encode() const noexcept
    {
        return WithCodecs::encode();
    }

    template<typename FormatContext>
    auto format(std::string_view const& spec, FormatContext& context) const
    {
        return fmt::format_to(context.out(), spec);
    }
};

constexpr auto operator==(NullaryOperands const& lhs, NullaryOperands const& rhs) noexcept
{
    (void)lhs;
    (void)rhs;
    return true;
}

constexpr auto operator!=(NullaryOperands const& lhs, NullaryOperands const& rhs) noexcept
{
    return !(lhs == rhs);
}

struct AOperands : detail::BaseOperands<AOperands>::WithTypes<Address>::WithCodecs<AddressOperand>
{
    constexpr AOperands(Address const address) noexcept : address{address} {}

    constexpr auto encode() const noexcept
    {
        return WithCodecs::encode(address);
    }

    template<typename FormatContext>
    auto format(std::string_view const& spec, FormatContext& context) const
    {
        return fmt::format_to(
            context.out(), spec, fmt::arg("address", fmt::format("{:03X}h", address)));
    }

    Address address;
};

constexpr auto operator==(AOperands const& lhs, AOperands const& rhs) noexcept
{
    return lhs.address == rhs.address;
}

constexpr auto operator!=(AOperands const& lhs, AOperands const& rhs) noexcept
{
    return !(lhs == rhs);
}

struct VOperands : detail::BaseOperands<VOperands>::WithTypes<Register>::WithCodecs<VxOperand>
{
    constexpr VOperands(Register const vx) noexcept : vx{vx} {}

    constexpr auto encode() const noexcept
    {
        return WithCodecs::encode(vx);
    }

    template<typename FormatContext>
    auto format(std::string_view const& spec, FormatContext& context) const
    {
        return fmt::format_to(context.out(), spec, fmt::arg("Vx", vx));
    }

    Register vx;
};

constexpr auto operator==(VOperands const& lhs, VOperands const& rhs) noexcept
{
    return lhs.vx == rhs.vx;
}

constexpr auto operator!=(VOperands const& lhs, VOperands const& rhs) noexcept
{
    return !(lhs == rhs);
}

struct VBOperands
    : detail::BaseOperands<VBOperands>::WithTypes<Register, Byte>::WithCodecs<VxOperand,
          ByteOperand>
{
    constexpr VBOperands(Register const vx, Byte const byte) noexcept : vx{vx}, byte{byte} {}

    constexpr auto encode() const noexcept
    {
        return WithCodecs::encode(vx, byte);
    }

    template<typename FormatContext>
    auto format(std::string_view const& spec, FormatContext& context) const
    {
        return fmt::format_to(context.out(), spec, fmt::arg("Vx", vx),
            fmt::arg("byte", fmt::format("{:02X}h", byte)));
    }

    Register vx;
    Byte byte;
};

constexpr auto operator==(VBOperands const& lhs, VBOperands const& rhs) noexcept
{
    return lhs.vx == rhs.vx && lhs.byte == rhs.byte;
}

constexpr auto operator!=(VBOperands const& lhs, VBOperands const& rhs) noexcept
{
    return !(lhs == rhs);
}

struct VVOperands
    : detail::BaseOperands<VVOperands>::WithTypes<Register, Register>::WithCodecs<VxOperand,
          VyOperand>
{
    constexpr VVOperands(Register const vx, Register const vy) noexcept : vx{vx}, vy{vy} {}

    constexpr auto encode() const noexcept
    {
        return WithCodecs::encode(vx, vy);
    }

    template<typename FormatContext>
    auto format(std::string_view const& spec, FormatContext& context) const
    {
        return fmt::format_to(context.out(), spec, fmt::arg("Vx", vx), fmt::arg("Vy", vy));
    }

    Register vx;
    Register vy;
};

constexpr auto operator==(VVOperands const& lhs, VVOperands const& rhs) noexcept
{
    return lhs.vx == rhs.vx && lhs.vy == rhs.vy;
}

constexpr auto operator!=(VVOperands const& lhs, VVOperands const& rhs) noexcept
{
    return !(lhs == rhs);
}

struct VVNOperands
    : detail::BaseOperands<VVNOperands>::WithTypes<Register, Register,
          Nibble>::WithCodecs<VxOperand, VyOperand, NibbleOperand>
{
    constexpr VVNOperands(Register const vx, Register const vy, Nibble const nibble) noexcept
        : vx{vx}, vy{vy}, nibble{nibble}
    {}

    constexpr auto encode() const noexcept
    {
        return WithCodecs::encode(vx, vy, nibble);
    }

    template<typename FormatContext>
    auto format(std::string_view const& spec, FormatContext& context) const
    {
        return fmt::format_to(context.out(), spec, fmt::arg("Vx", vx), fmt::arg("Vy", vy),
            fmt::arg("nibble", fmt::format("{:01X}h", nibble)));
    }

    Register vx;
    Register vy;
    Nibble nibble;
};

constexpr auto operator==(VVNOperands const& lhs, VVNOperands const& rhs) noexcept
{
    return lhs.vx == rhs.vx && lhs.vy == rhs.vy && lhs.nibble == rhs.nibble;
}

constexpr auto operator!=(VVNOperands const& lhs, VVNOperands const& rhs) noexcept
{
    return !(lhs == rhs);
}

using Operands =
    std::variant<NullaryOperands, AOperands, VOperands, VBOperands, VVOperands, VVNOperands>;

} // namespace libnpln::machine

#endif
