#include <benchmark/benchmark.h>

#include "book_database.hpp"
#include "statistics.hpp"

using namespace bookdb;

static BookDatabase<> GenerateDB(size_t n) {
    BookDatabase<> db;

    for (size_t i = 0; i < n; ++i) {
        db.EmplaceBack("Book", std::format("Author{}", i % 1000), 2000 + (i % 20), Genre::Fiction, 4.5, 100);
    }

    return db;
}

static void BM_HistogramStd(benchmark::State &state) {
    auto db = GenerateDB(state.range(0));

    for (auto _ : state) {
        auto result = buildAuthorHistogram(db);
        benchmark::DoNotOptimize(result);
    }
}

static void BM_HistogramFlat(benchmark::State &state) {
    auto db = GenerateDB(state.range(0));

    for (auto _ : state) {
        auto result = buildAuthorHistogramFlat(db);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_HistogramStd)->Range(1'000, 1'000'000);

BENCHMARK(BM_HistogramFlat)->Range(1'000, 1'000'000);

BENCHMARK_MAIN();