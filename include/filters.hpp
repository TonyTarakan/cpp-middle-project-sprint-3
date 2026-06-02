#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

auto YearBetween(int from, int to) {
    return [from, to](const Book &b) { return b.year >= from && b.year <= to; };
}

auto RatingAbove(double rating) {
    return [rating](const Book &b) { return b.rating > rating; };
}

template <BookPredicate... Preds>
auto all_of(Preds... preds) {
    return [=](const Book &b) { return (preds(b) && ...); };
}

template <BookPredicate... Preds>
auto any_of(Preds... preds) {
    return [=](const Book &b) { return (preds(b) || ...); };
}

template <BookIterator It, BookSentinel<It> S, BookPredicate... Preds>
auto filterBooks(It begin, S end, Preds... preds) {
    std::vector<std::reference_wrapper<const Book>> result;

    auto combined_pred = all_of(preds...);
    for (auto it = begin; it != end; ++it) {
        if (combined_pred(*it)) {
            result.emplace_back(std::cref(*it));
        }
    }

    return result;
}

}  // namespace bookdb