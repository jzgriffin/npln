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

#include <array>
#include <cstddef>
#include <optional>

namespace libnpln::machine {

class Display
{
public:
    using Pixel = bool;

    using Proxy = Pixel*;
    using ConstProxy = Pixel const*;

    constexpr auto pixel(std::size_t const x, std::size_t const y)
        const -> ConstProxy
    {
        if (auto const z = offset(x, y); z != std::nullopt) {
            return &pixels_[*z];
        }

        return nullptr;
    }

    constexpr auto pixel(std::size_t const x, std::size_t const y)
        -> Proxy
    {
        if (auto const z = offset(x, y); z != std::nullopt) {
            return &pixels_[*z];
        }

        return nullptr;
    }

    constexpr auto clear() noexcept -> void
    {
        // std::fill is not constexpr until C++20.
        for (auto& p : pixels_) {
            p = false;
        }
    }

    static constexpr std::size_t width = 64;
    static constexpr std::size_t height = 32;

private:
    static constexpr auto offset(std::size_t const x,
        std::size_t const y) -> std::optional<std::size_t>
    {
        return x < width && y < height
            ? std::optional{y * width + x}
            : std::nullopt;
    }

    std::array<Pixel, width * height> pixels_;
};

}

#endif
