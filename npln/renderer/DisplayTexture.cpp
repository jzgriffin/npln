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

#include <npln/renderer/DisplayTexture.hpp>

#include <gsl/gsl>

namespace {

constexpr gl::GLuint off_color = 0x000000FF;
constexpr gl::GLuint on_color = 0xFFFFFFFF;

} // namespace

namespace npln::renderer {

DisplayTexture::DisplayTexture(libnpln::machine::Display& display) : display_(display)
{
    update();
    texture_.image2D(0, format, width, height, 0, format, type, pixels.data());
    texture_.setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
    texture_.setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
}

auto DisplayTexture::update() -> void
{
    for (std::decay_t<decltype(height)> y = 0; y < height; ++y) {
        for (std::decay_t<decltype(width)> x = 0; x < width; ++x) {
            auto& pixel = gsl::at(pixels, gsl::narrow<gsl::index>(y * width + x));
            pixel = *display_.pixel(x, y) ? on_color : off_color;
        }
    }
}

auto DisplayTexture::render() -> void
{
    // TODO: Profile whether it is faster to upload the texture on every frame or only after
    // checking whether the display has actually changed.
    texture_.subImage2D(0, 0, 0, width, height, format, type, pixels.data());
}

} // namespace npln::renderer
