#include <gtest/gtest.h>

#include "book_database.hpp"

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
protected:
    BookDatabase<std::vector<Book>> db;

    void SetUp() override {
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
        db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    }
};

TEST_F(BookDatabaseTest, EmplaceBackIncreasesSize) {
    EXPECT_EQ(db.size(), 3);
    EXPECT_FALSE(db.empty());

    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    EXPECT_EQ(db.size(), 4);
}

TEST_F(BookDatabaseTest, EmptyDatabaseIsEmpty) {
    BookDatabase<std::vector<Book>> empty_db;
    EXPECT_TRUE(empty_db.empty());
    EXPECT_EQ(empty_db.size(), 0);
}

TEST_F(BookDatabaseTest, IteratorsCoversAllBooks) {
    int count = 0;
    for (const auto &book : db) {
        ++count;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(BookDatabaseTest, IteratorBeginPointsToFirstBook) { EXPECT_EQ(db.begin()->title, "1984"); }

TEST_F(BookDatabaseTest, ClearEmptiesDatabase) {
    db.clear();
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
}

TEST_F(BookDatabaseTest, ClearAlsoClearsAuthors) {
    db.clear();
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST_F(BookDatabaseTest, AuthorsAreUnique) {
    // George Orwell добавлен дважды — в авторах должен быть один раз
    const auto &authors = db.GetAuthors();
    EXPECT_EQ(authors.size(), 2);  // George Orwell, Harper Lee
}

TEST_F(BookDatabaseTest, AuthorsContainExpectedNames) {
    const auto &authors = db.GetAuthors();
    EXPECT_TRUE(authors.contains("George Orwell"));
    EXPECT_TRUE(authors.contains("Harper Lee"));
    EXPECT_FALSE(authors.contains("Jane Austen"));
}