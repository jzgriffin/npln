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

#include <gsl/gsl>

#include <algorithm>

namespace libnpln::machine {

Display::Display() : pixels_(std::make_unique<Pixels>()) {}

Display::Display(Display const& other) : pixels_(std::make_unique<Pixels>(*other.pixels_)) {}

// This move constructor cannot be noexcept because it must allocate memory.
// NOLINTNEXTLINE(performance-noexcept-move-constructor, hicpp-noexcept-move)
Display::Display(Display&& other) : pixels_(std::make_unique<Pixels>(*other.pixels_))
{
    // Decays to copy semantics because pixels_ cannot be moved from.
}

auto Display::operator=(Display const& other) -> Display&
{
    if (this == &other) {
        return *this;
    }

    *pixels_ = *other.pixels_;
    return *this;
}

auto Display::operator=(Display&& other) noexcept -> Display&
{
    if (this == &other) {
        return *this;
    }

    // Decays to copy semantics because pixels_ cannot be moved from.
    *pixels_ = *other.pixels_;
    return *this;
}

auto Display::operator==(Display const& rhs) const noexcept -> bool
{
    return *pixels_ == *rhs.pixels_;
}

auto Display::operator!=(Display const& rhs) const noexcept -> bool
{
    return !(*this == rhs);
}

auto Display::pixel(std::size_t const x, std::size_t const y) const -> ConstProxy
{
    if (auto const z = offset(x, y); z != std::nullopt) {
        return &gsl::at(*pixels_, gsl::narrow<gsl::index>(*z));
    }

    return nullptr;
}

auto Display::pixel(std::size_t const x, std::size_t const y) -> Proxy
{
    if (auto const z = offset(x, y); z != std::nullopt) {
        return &gsl::at(*pixels_, gsl::narrow<gsl::index>(*z));
    }

    return nullptr;
}

auto Display::clear() noexcept -> void
{
    std::fill(std::begin(*pixels_), std::end(*pixels_), false);
}

} // namespace libnpln::machine
