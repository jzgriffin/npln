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

#ifndef LIBNPLN_MACHINE_DISPLAY_HPP
#define LIBNPLN_MACHINE_DISPLAY_HPP

#include <fmt/format.h>
#include <gsl/gsl>

#include <array>
#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>

namespace libnpln::machine {

class Display
{
public:
    using Pixel = bool;

    using Proxy = Pixel*;
    using ConstProxy = Pixel const*;

    Display();
    Display(Display const& other);
    Display(Display&& other);

    auto operator=(Display const& other) -> Display&;
    auto operator=(Display&& other) noexcept -> Display&;

    auto operator==(Display const& rhs) const noexcept -> bool;
    auto operator!=(Display const& rhs) const noexcept -> bool;

    auto pixel(std::size_t const x, std::size_t const y) const -> ConstProxy;
    auto pixel(std::size_t const x, std::size_t const y) -> Proxy;

    auto clear() noexcept -> void;

    static constexpr std::size_t width = 64;
    static constexpr std::size_t height = 32;

private:
    using Pixels = std::array<Pixel, width * height>;

    static constexpr auto offset(std::size_t const x, std::size_t const y)
        -> std::optional<std::size_t>
    {
        return x < width && y < height ? std::optional{y * width + x} : std::nullopt;
    }

    gsl::not_null<std::unique_ptr<Pixels>> const pixels_;
};

} // namespace libnpln::machine

template<>
struct fmt::formatter<libnpln::machine::Display>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(libnpln::machine::Display const& value, FormatContext& context)
    {
        auto out = context.out();
        for (std::size_t y = 0; y < value.height; ++y) {
            if (y > 0) {
                out = format_to(out, "\n");
            }
            for (std::size_t x = 0; x < value.width; ++x) {
                out = format_to(out, "{}", *value.pixel(x, y) ? "X" : ".");
            }
        }
        return out;
    }
};

#endif
