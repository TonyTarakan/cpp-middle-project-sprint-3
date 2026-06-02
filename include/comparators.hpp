#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
    bool operator()(const Book &lhs, std::string_view rhs) const { return lhs.author < rhs; }
    bool operator()(std::string_view lhs, const Book &rhs) const { return lhs < rhs.author; }
};

struct LessByTitle {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title < rhs.title; }
    bool operator()(const Book &lhs, std::string_view rhs) const { return lhs.title < rhs; }
    bool operator()(std::string_view lhs, const Book &rhs) const { return lhs < rhs.title; }
};

struct LessByYear {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year < rhs.year; }
    bool operator()(const Book &lhs, int rhs) const { return lhs.year < rhs; }
    bool operator()(int lhs, const Book &rhs) const { return lhs < rhs.year; }
};

struct LessByGenre {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.genre < rhs.genre; }
    bool operator()(const Book &lhs, Genre rhs) const { return lhs.genre < rhs; }
    bool operator()(Genre lhs, const Book &rhs) const { return lhs < rhs.genre; }
};

struct LessByRating {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
    bool operator()(const Book &lhs, double rhs) const { return lhs.rating < rhs; }
    bool operator()(double lhs, const Book &rhs) const { return lhs < rhs.rating; }
};

struct LessByPopularity {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const {
        return lhs.rating * lhs.read_count < rhs.rating * rhs.read_count;
    }
};

struct LessByReadCount {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
    bool operator()(const Book &lhs, int rhs) const { return lhs.read_count < rhs; }
    bool operator()(int lhs, const Book &rhs) const { return lhs < rhs.read_count; }
};

}  // namespace bookdb::comp