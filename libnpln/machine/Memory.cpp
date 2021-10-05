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

#include <libnpln/machine/Memory.hpp>

#include <gsl/narrow>

#include <fstream>

namespace libnpln::machine {

auto load_into_memory(std::istream& s, Memory& m, Address const a) -> bool
{
    if (m.size() < a) {
        return false;
    }

    // reinterpret_cast between unsigned char* and char* is safe because they have the same
    // representation and alignment.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    s.read(reinterpret_cast<char*>(std::next(m.data(), a)),
        gsl::narrow<std::streamsize>(m.size() - a));
    return s.fail() && s.eof() && !s.bad();
}

auto load_into_memory(std::filesystem::path const& p, Memory& m, Address const a) -> bool
{
    auto s = std::ifstream{p, std::ios::in | std::ios::binary};
    return s ? load_into_memory(s, m, a) : false;
}

} // namespace libnpln::machine
