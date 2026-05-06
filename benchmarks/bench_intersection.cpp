#include <ngine/core/circle.hpp>
#include <ngine/core/line.hpp>
#include <ngine/intersection/intersection_engine.hpp>

#include <benchmark/benchmark.h>

using namespace ngine;

static void BM_LineLineIntersection(benchmark::State& state) {
    IntersectionEngine engine;
    auto l1 = Line::from_points(Point(0, 0), Point(1, 1));
    auto l2 = Line::from_points(Point(0, 1), Point(1, 0));

    for (auto _ : state) {
        auto result = engine.intersect(l1, l2);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_LineLineIntersection);

static void BM_LineCircleIntersection(benchmark::State& state) {
    IntersectionEngine engine;
    auto line = Line::from_points(Point(-10, 0), Point(10, 0));
    Circle circle(Point(0, 0), 5.0);

    for (auto _ : state) {
        auto result = engine.intersect(line, circle);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_LineCircleIntersection);

static void BM_CircleCircleIntersection(benchmark::State& state) {
    IntersectionEngine engine;
    Circle c1(Point(0, 0), 5.0);
    Circle c2(Point(6, 0), 5.0);

    for (auto _ : state) {
        auto result = engine.intersect(c1, c2);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CircleCircleIntersection);

BENCHMARK_MAIN();
