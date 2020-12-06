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

#include <libnpln/machine/Keys.hpp>

#include <libnpln/detail/ContainerCast.hpp>
#include <libnpln/utility/BitSetDifference.hpp>

namespace libnpln::machine {

auto keys_difference(Keys const& initial, Keys const& final) noexcept
    -> std::pair<std::set<Key>, std::set<Key>>
{
    using detail::static_cast_set;
    auto const [pbs, rbs] = utility::bit_set_split_difference(initial, final);
    return {static_cast_set<Key>(pbs), static_cast_set<Key>(rbs)};
}

} // namespace libnpln::machine
