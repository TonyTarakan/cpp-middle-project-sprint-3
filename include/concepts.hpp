#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookIterator = requires(T it) {
    { *it } -> std::convertible_to<Book>;
    { ++it } -> std::same_as<T &>;
    { it == it } -> std::convertible_to<bool>;
    { it != it } -> std::convertible_to<bool>;
};

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<I, S>;

template <typename T>
concept BookContainerLike = requires(T c) {
    { c.begin() } -> BookIterator;
    { c.end() } -> BookSentinel<typename T::iterator>;
    { c.size() } -> std::convertible_to<size_t>;
    { c.empty() } -> std::convertible_to<bool>;
    { c.clear() };
};

template <typename P>
concept BookPredicate = requires(P p) {
    { p(std::declval<Book>()) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C cmp, const Book &b1, const Book &b2) {
    { cmp(b1, b2) } -> std::convertible_to<bool>;
};

}  // namespace bookdb