#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>

template <typename K, typename V, typename Comp, typename KC, typename VC>
struct std::formatter<std::flat_map<K, V, Comp, KC, VC>> {
    template <typename FormatContext>
    auto format(const std::flat_map<K, V, Comp, KC, VC> &map, FormatContext &fc) const {
        for (const auto &[key, value] : map) {
            std::format_to(fc.out(), "  {}: {}\n", key, value);
        }
        return fc.out();
    }

    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
};

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, size_t, Comparator> histogram;
    for (const auto &book : cont.GetBooks()) {
        ++histogram[std::string{book.author}];
    }
    return histogram;
}

// Средний рейтинг книг по жанрам, используя один или несколько из доступных flat-контейнеров
auto calculateGenreRatings(const auto &begin, const auto &end) {
    std::flat_map<Genre, double> ratings;
    std::flat_map<Genre, int> counts;
    for (auto book = begin; book != end; ++book) {
        ++counts[book->genre];
        ratings[book->genre] += book->rating;
    }
    for (auto [genre, rating] : ratings) {
        rating /= counts.at(genre);
    }
    return ratings;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.empty())
        return 0.0;
    double sum =
        std::accumulate(cont.begin(), cont.end(), 0.0, [](double acc, const Book &book) { return acc + book.rating; });
    return sum / cont.size();
}

}  // namespace bookdb
