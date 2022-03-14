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

#ifndef NPLN_RENDERER_DISPLAYTEXTURE_HPP
#define NPLN_RENDERER_DISPLAYTEXTURE_HPP

#include <libnpln/machine/Display.hpp>

#include <glbinding/gl/gl.h>
#include <globjects/Texture.h>

#include <array>
#include <type_traits>

namespace npln::renderer {

class DisplayTexture
{
public:
    explicit DisplayTexture(libnpln::machine::Display& display);
    DisplayTexture(DisplayTexture const&) = delete;
    DisplayTexture(DisplayTexture&&) noexcept = delete;
    ~DisplayTexture() = default;

    auto operator=(DisplayTexture const&) -> DisplayTexture& = delete;
    auto operator=(DisplayTexture&&) noexcept -> DisplayTexture& = delete;

    auto display() noexcept -> libnpln::machine::Display&
    {
        return display_;
    }
    [[nodiscard]] auto display() const noexcept -> libnpln::machine::Display const&
    {
        return display_;
    }

    auto texture() noexcept -> globjects::Texture&
    {
        return texture_;
    }
    [[nodiscard]] auto texture() const noexcept -> globjects::Texture const&
    {
        return texture_;
    }

    auto update() -> void;
    auto render() -> void;

private:
    libnpln::machine::Display& display_;

    static constexpr auto width = std::decay_t<decltype(display_)>::width;
    static constexpr auto height = std::decay_t<decltype(display_)>::height;
    static constexpr auto format = gl::GL_RGBA;
    static constexpr auto type = gl::GL_UNSIGNED_INT_8_8_8_8;

    std::array<gl::GLuint, width * height> pixels{};

    globjects::Texture texture_;
};

} // namespace npln::renderer

#endif
