#include <ngine/spatial/quadtree.hpp>

#include <benchmark/benchmark.h>

#include <random>

using namespace ngine;

static void BM_QuadtreeInsert(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<Real> dist(-500, 500);

    for (auto _ : state) {
        Quadtree qt;
        for (int i = 0; i < state.range(0); ++i) {
            Real x = dist(rng);
            Real y = dist(rng);
            qt.insert(static_cast<EntityId>(i + 1), BoundingBox(x, y, x + 1, y + 1));
        }
        benchmark::DoNotOptimize(qt.size());
    }
}
BENCHMARK(BM_QuadtreeInsert)->Range(10, 10000);

static void BM_QuadtreeQuery(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<Real> dist(-500, 500);

    Quadtree qt;
    for (int i = 0; i < state.range(0); ++i) {
        Real x = dist(rng);
        Real y = dist(rng);
        qt.insert(static_cast<EntityId>(i + 1), BoundingBox(x, y, x + 1, y + 1));
    }

    for (auto _ : state) {
        auto results = qt.query(BoundingBox(-50, -50, 50, 50));
        benchmark::DoNotOptimize(results);
    }
}
BENCHMARK(BM_QuadtreeQuery)->Range(10, 10000);

BENCHMARK_MAIN();
