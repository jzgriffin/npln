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

#ifndef LIBNPLN_UTILITY_NUMERIC_HPP
#define LIBNPLN_UTILITY_NUMERIC_HPP

#include <limits>
#include <type_traits>

namespace libnpln::utility {

template<typename T>
auto addition_overflow(T const x, T const y) -> std::enable_if_t<std::is_integral_v<T>, bool>
{
    return y > 0 && x > std::numeric_limits<T>::max() - y;
}

template<typename T>
auto subtraction_underflow(T const x, T const y) -> std::enable_if_t<std::is_integral_v<T>, bool>
{
    return y > 0 && x < std::numeric_limits<T>::min() + y;
}

template<typename T>
auto lsb(T const x) -> std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool>
{
    return (x & (1 << 0)) != 0;
}

template<typename T>
auto msb(T const x) -> std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool>
{
    return (x & (1 << (std::numeric_limits<T>::digits - 1))) != 0;
}

} // namespace libnpln::utility

#endif
