#include <gtest/gtest.h>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
protected:
    BookDatabase<std::vector<Book>> db;

    void SetUp() override {
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
        db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
        db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    }
};

// BookDatabase

TEST_F(BookDatabaseTest, EmplaceBackIncreasesSize) {
    EXPECT_EQ(db.size(), 6);
    EXPECT_FALSE(db.empty());

    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    EXPECT_EQ(db.size(), 7);
}

TEST_F(BookDatabaseTest, EmptyDatabaseIsEmpty) {
    BookDatabase<std::vector<Book>> empty_db;
    EXPECT_TRUE(empty_db.empty());
    EXPECT_EQ(empty_db.size(), 0);
}

TEST_F(BookDatabaseTest, IteratorCoversAllBooks) {
    int count = 0;
    for (const auto &book : db) {
        ++count;
    }
    EXPECT_EQ(count, 6);
}

TEST_F(BookDatabaseTest, IteratorBeginPointsToFirstBook) { EXPECT_EQ(db.begin()->title, "1984"); }

TEST_F(BookDatabaseTest, ClearEmptiesDatabase) {
    db.clear();
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST_F(BookDatabaseTest, AuthorsAreUnique) {
    EXPECT_EQ(db.GetAuthors().size(), 5);  // George Orwell добавлен дважды
}

TEST_F(BookDatabaseTest, AuthorStringViewNotDangling) {
    // string_view в Book должна ссылаться на строку в authors_, а не на временную
    auto it = db.begin();
    std::string_view author = it->author;
    EXPECT_EQ(author, "George Orwell");
}

// buildAuthorHistogramFlat

TEST_F(BookDatabaseTest, HistogramCountsCorrectly) {
    auto hist = buildAuthorHistogramFlat(db);
    EXPECT_EQ(hist["George Orwell"], 2);
    EXPECT_EQ(hist["Harper Lee"], 1);
    EXPECT_EQ(hist["J.R.R. Tolkien"], 1);
}

TEST_F(BookDatabaseTest, HistogramEmptyDatabase) {
    BookDatabase<std::vector<Book>> empty_db;
    auto hist = buildAuthorHistogramFlat(empty_db);
    EXPECT_TRUE(hist.empty());
}

// calculateAverageRating

TEST_F(BookDatabaseTest, AverageRatingCorrect) {
    double avg = calculateAverageRating(db);
    double expected = (4.0 + 4.4 + 4.8 + 4.5 + 4.9 + 4.2) / 6.0;
    EXPECT_NEAR(avg, expected, 1e-9);
}

TEST_F(BookDatabaseTest, AverageRatingEmptyDatabase) {
    BookDatabase<std::vector<Book>> empty_db;
    EXPECT_EQ(calculateAverageRating(empty_db), 0.0);
}

// calculateGenreRatings

TEST_F(BookDatabaseTest, GenreRatingsCorrect) {
    auto ratings = calculateGenreRatings(db.begin(), db.end());
    // SciFi: (4.0 + 4.5) / 2 = 4.25
    EXPECT_NEAR(ratings[Genre::SciFi], 4.25, 1e-9);
    // Fiction: (4.4 + 4.8 + 4.9 + 4.2) / 4 = 4.575
    EXPECT_NEAR(ratings[Genre::Fiction], 4.575, 1e-9);
}

// filterBooks

TEST_F(BookDatabaseTest, FilterByYearBetween) {
    auto result = filterBooks(db.begin(), db.end(), YearBetween(1940, 1960));
    EXPECT_EQ(result.size(), 4);  // 1984(1949), Animal Farm(1945), To Kill a Mockingbird(1960), Lord of the Flies(1954)
}

TEST_F(BookDatabaseTest, FilterByRatingAbove) {
    auto result = filterBooks(db.begin(), db.end(), RatingAbove(4.5));
    EXPECT_EQ(result.size(), 2);  // To Kill a Mockingbird(4.8), The Hobbit(4.9)
}

TEST_F(BookDatabaseTest, FilterAllOf) {
    auto result = filterBooks(db.begin(), db.end(), all_of(YearBetween(1940, 1960), RatingAbove(4.5)));
    // To Kill a Mockingbird(1960, 4.8)
    EXPECT_EQ(result.size(), 1);
}

TEST_F(BookDatabaseTest, FilterAnyOf) {
    auto result = filterBooks(db.begin(), db.end(), any_of(YearBetween(1930, 1935), RatingAbove(4.8)));
    // Brave New World(1932), The Hobbit(4.9)
    EXPECT_EQ(result.size(), 2);
}

TEST_F(BookDatabaseTest, FilterNoResults) {
    auto result = filterBooks(db.begin(), db.end(), RatingAbove(5.0));
    EXPECT_TRUE(result.empty());
}

TEST_F(BookDatabaseTest, FilterEmptyDatabase) {
    BookDatabase<std::vector<Book>> empty_db;
    auto result = filterBooks(empty_db.begin(), empty_db.end(), RatingAbove(4.0));
    EXPECT_TRUE(result.empty());
}

TEST_F(BookDatabaseTest, FilterReturnsReferences) {
    auto result = filterBooks(db.begin(), db.end(), RatingAbove(4.7));
    ASSERT_FALSE(result.empty());
    // проверяем что это ссылки на оригинальные объекты, а не копии
    EXPECT_EQ(&result[0].get(), &*std::find_if(db.begin(), db.end(), [](const Book &b) { return b.rating > 4.7; }));
}

// getTopNBy

TEST_F(BookDatabaseTest, GetTopNByRating) {
    auto top3 = getTopNBy(db, 3, comp::LessByRating{});
    EXPECT_EQ(top3.size(), 3);
    double min_top = std::min({top3[0].get().rating, top3[1].get().rating, top3[2].get().rating});
    for (const auto &ref : top3) {
        EXPECT_GE(ref.get().rating, min_top);
    }
}

TEST_F(BookDatabaseTest, GetTopNGreaterThanSize) { EXPECT_NO_THROW(getTopNBy(db, 6, comp::LessByRating{})); }

// sampleRandomBooks

TEST_F(BookDatabaseTest, SampleReturnsCorrectCount) {
    auto sample = sampleRandomBooks(db, 3);
    EXPECT_EQ(sample.size(), 3);
}

TEST_F(BookDatabaseTest, SampleEmptyDatabase) {
    BookDatabase<std::vector<Book>> empty_db;
    auto sample = sampleRandomBooks(empty_db, 3);
    EXPECT_TRUE(sample.empty());
}

TEST_F(BookDatabaseTest, SampleMoreThanSizeReturnsAll) {
    auto sample = sampleRandomBooks(db, 100);
    EXPECT_EQ(sample.size(), db.size());
}