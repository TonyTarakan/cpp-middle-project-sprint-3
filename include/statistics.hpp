#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"
#include "comparators.hpp"

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

template <BookContainerLike T>
auto buildAuthorHistogram(const BookDatabase<T> &cont) {
    std::unordered_map<std::string_view, size_t> histogram;
    for (const auto &book : cont.GetBooks()) {
        ++histogram[std::string{book.author}];
    }
    return histogram;
}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, size_t, Comparator> histogram;
    for (const auto &book : cont.GetBooks()) {
        ++histogram[std::string{book.author}];
    }
    return histogram;
}

// Средний рейтинг книг по жанрам, используя один или несколько из доступных flat-контейнеров
template <BookIterator Iter>
auto calculateGenreRatings(Iter begin, Iter end) {
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

inline double calculateAverageRating(std::span<const Book> books) {
    if (books.empty())
        return 0.0;
    double sum =
        std::accumulate(books.begin(), books.end(), 0.0, [](double acc, const Book &b) { return acc + b.rating; });
    return sum / books.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &db, size_t n) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::sample(db.begin(), db.end(), std::back_inserter(result), n, std::mt19937{std::random_device{}()});
    return result;
}

// Функция getTopNBy должна выбирать из библиотеки указанное количество книг c наивысшим рейтингом и
// возвращать их в виде std::vector<std::reference_wrapper<const Book>>.
// Это единственная функция, которой разрешено изменять переданный контейнер.
template <BookContainerLike T, BookComparator Cmp>
auto getTopNBy(BookDatabase<T> &db, size_t n, Cmp cmp) {
    n = std::min(n, db.size());
    std::partial_sort(db.begin(), db.begin() + n, db.end(), cmp);
    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(n);
    for (auto it = db.begin(); it != db.begin() + n; ++it) {
        result.emplace_back(std::cref(*it));
    }
    return result;
}

}  // namespace bookdb
