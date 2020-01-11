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

#ifndef LIBNPLN_UTILITY_FIXEDSIZESTACK_HPP
#define LIBNPLN_UTILITY_FIXEDSIZESTACK_HPP

#include <array>
#include <optional>

namespace libnpln::utility {

template<typename TValue, std::size_t TMaxSize>
class FixedSizeStack
{
public:
    using container_type = std::array<TValue, TMaxSize>;
    using value_type = typename container_type::value_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using difference_type = typename container_type::difference_type;
    using size_type = typename container_type::size_type;
    using const_iterator = typename container_type::const_iterator;

    constexpr auto operator==(FixedSizeStack const& rhs) const noexcept
    { return elements_ == rhs.elements_ && size_ == rhs.size_; }
    constexpr auto operator!=(FixedSizeStack const& rhs) const noexcept
    { return !(*this == rhs); }

    constexpr auto swap(FixedSizeStack& rhs) noexcept -> void
    {
        using std::swap;
        swap(elements_, rhs.elements_);
        swap(size_, rhs.size_);
    }

    constexpr auto begin() const noexcept -> const_iterator
    { return std::begin(elements_); };
    constexpr auto end() const noexcept -> const_iterator
    { return std::next(begin(), size()); };

    constexpr auto cbegin() const noexcept { return begin(); }
    constexpr auto cend() const noexcept { return end(); }

    static constexpr auto max_size() noexcept -> size_type
    { return TMaxSize; }
    constexpr auto size() const noexcept { return size_; }
    [[nodiscard]] constexpr auto empty() const noexcept
    { return size_ == 0; }
    [[nodiscard]] constexpr auto full() const noexcept
    { return size_ == max_size(); }

    constexpr auto top() const noexcept -> std::optional<value_type>
    {
        if (empty()) {
            return std::nullopt;
        }

        return elements_[size_ - 1];
    }

    template<typename... TArgs>
    constexpr auto emplace(TArgs&&... args) noexcept -> bool
    {
        if (full()) {
            return false;
        }

        elements_[size_++] = value_type{std::forward<TArgs>(args)...};
        return true;
    }

    constexpr auto push(value_type const& x) noexcept -> bool
    {
        if (full()) {
            return false;
        }

        elements_[size_++] = x;
        return true;
    }

    constexpr auto push(value_type&& x) noexcept -> bool
    {
        if (full()) {
            return false;
        }

        elements_[size_++] = std::move(x);
        return true;
    }

    constexpr auto pop() noexcept -> std::optional<value_type>
    {
        if (empty()) {
            return std::nullopt;
        }

        return elements_[--size_];
    }

private:
    std::array<value_type, max_size()> elements_;
    size_type size_ = 0;
};

template<typename TValue, std::size_t TMaxSize>
constexpr auto swap(FixedSizeStack<TValue, TMaxSize>& lhs,
    FixedSizeStack<TValue, TMaxSize>& rhs) noexcept -> void
{
    lhs.swap(rhs);
}

}

#endif
