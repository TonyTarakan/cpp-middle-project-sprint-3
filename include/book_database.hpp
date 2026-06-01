#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = Book;
    using reference = Book &;
    using const_reference = const Book &;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using size_type = typename BookContainer::size_type;
    using difference_type = typename BookContainer::difference_type;

    // Ваш код здесь

    using AuthorContainer = std::vector<std::string>;  // TODO: make it depend on BookContainer

    BookDatabase() = default;

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    BookContainer &GetBooks() { return books_; }
    const BookContainer &GetBooks() const { return books_; }

    AuthorContainer &GetAuthors() { return authors_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

    // Standard container interface methods

    iterator begin() { return books_.begin(); }
    const_iterator begin() const { return books_.begin(); }
    const_iterator cbegin() const { return books_.cbegin(); }
    iterator end() { return books_.end(); }
    const_iterator end() const { return books_.end(); }
    const_iterator cend() const { return books_.cend(); }

    size_type size() const { return books_.size(); }
    bool empty() const { return books_.empty(); }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {

template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
