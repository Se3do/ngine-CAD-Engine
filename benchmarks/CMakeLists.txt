# Benchmarks are optional - enable with -DNGINE_BUILD_BENCHMARKS=ON
# Requires Google Benchmark (fetched automatically)

if(NGINE_BUILD_BENCHMARKS)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
        GIT_SHALLOW TRUE
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(benchmark)

    add_executable(bench_intersection bench_intersection.cpp)
    target_link_libraries(bench_intersection PRIVATE ngine_intersection benchmark::benchmark)

    add_executable(bench_quadtree bench_quadtree.cpp)
    target_link_libraries(bench_quadtree PRIVATE ngine_spatial benchmark::benchmark)
endif()
