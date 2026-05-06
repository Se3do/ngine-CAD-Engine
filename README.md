# ngine: CAD Engine

A production-quality C++20 geometry engine with constraint solving, intersection detection, and extensible architecture. Designed to demonstrate engineering software development practices found in CAD/PLM systems.

## Features

- **Geometry Core** — Point, Vector2D, Line, Segment, Circle, Polygon with full const-correctness and numerical precision handling
- **Intersection Engine** — Strategy-pattern based intersection detection (line-line, line-circle, circle-circle, segment-segment) with robust edge-case handling
- **Constraint Solver** — Pluggable solver architecture with sequential (Gauss-Seidel) implementation. Supports coincident, parallel, perpendicular, fixed-distance, and point-on-line constraints
- **Command System** — Full undo/redo via Command pattern with configurable history depth
- **Spatial Indexing** — Quadtree-based spatial partitioning for efficient range and radius queries
- **Serialization** — JSON-based scene persistence via nlohmann/json
- **CLI Interface** — Interactive REPL for geometry creation, intersection queries, and constraint solving
- **Logging** — Thread-safe centralized logging with configurable sinks and levels

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                   Interface Layer                     │
│              (CLI / REST / Document)                  │
├─────────────────────────────────────────────────────┤
│  Commands  │  Constraints  │  Serialization  │ Spatial│
├─────────────────────────────────────────────────────┤
│              Intersection Engine                      │
├─────────────────────────────────────────────────────┤
│                 Geometry Core                         │
├─────────────────────────────────────────────────────┤
│                    Logging                            │
└─────────────────────────────────────────────────────┘
```

Dependencies flow downward. Each layer is a separate CMake library target.

## Quick Start

### Prerequisites

- CMake 3.22+
- C++20 compiler (Clang 18+, GCC 13+, MSVC 2022+)
- Ninja (recommended)
- Docker (optional)

### Build

```bash
cmake --preset debug
cmake --build build/debug
```

### Test

```bash
ctest --test-dir build/debug --output-on-failure
```

### Run

```bash
./build/debug/apps/ngine_cli
```

### Docker

```bash
# Full build + test
docker compose run --rm test

# Development shell
docker compose run --rm dev

# Static analysis
docker compose run --rm tidy

# Format check
docker compose run --rm format-check
```

## CLI Usage

```
ngine> CREATE_POINT 0 0
OK: Created point #1 at (0, 0)

ngine> CREATE_LINE 0 0 10 10
OK: Created line #2 through (0,0) and (10,10)

ngine> CREATE_LINE 0 10 10 0
OK: Created line #3 through (0,10) and (10,0)

ngine> INTERSECT 2 3
Intersection: 1 point(s)
  (5, 5)

ngine> MOVE 1 3 4
OK: Moved entity #1 by (3, 4)

ngine> UNDO
OK: Undo

ngine> SOLVE
OK: Converged in 0 iterations (error: 0.00e+00)
```

## Design Decisions

| Decision | Rationale |
|----------|-----------|
| Line as ax+by+c=0 (normalized) | Numerically stable for intersections; signed distance is free |
| `std::variant` for entity storage | Value semantics, no virtual dispatch overhead for primitives |
| Strategy pattern for intersections | Open/closed principle — new algorithms without modifying existing code |
| Pluggable solver interface | Sequential solver now, Newton-Raphson later without touching constraints |
| Separate library targets | Independent compilation, testing, and dependency tracking |
| Shared_ptr for constraint targets | Constraints observe and modify geometry; shared ownership is natural |

## Project Structure

```
include/ngine/     Public headers organized by module
src/               Implementation files with per-module CMakeLists.txt
apps/              Executable entry points
tests/             GoogleTest suites per module + integration
benchmarks/        Google Benchmark performance tests
cmake/             Build system helpers (warnings, sanitizers, deps)
```

## Build Presets

| Preset | Purpose |
|--------|---------|
| `debug` | Development build with tests |
| `release` | Optimized build |
| `sanitize` | ASan + UBSan enabled |
| `clang-tidy` | Static analysis during build |

## Tooling

- **clang-format** — Google-based style, 100 col limit, 4-space indent
- **clang-tidy** — Bugprone, cppcoreguidelines, modernize, performance, readability checks
- **Sanitizers** — AddressSanitizer + UndefinedBehaviorSanitizer
- **CI** — GitHub Actions: build, test, format-check, clang-tidy, sanitizer runs

## Extending

### Adding a new geometry type

1. Add header in `include/ngine/core/`
2. Add to `GeometryEntity` variant in `commands/geometry_commands.hpp`
3. Add serialization support in `json_serializer.cpp`
4. Add intersection support if applicable

### Adding a new constraint

1. Inherit from `Constraint` in `constraints/constraint_types.hpp`
2. Implement `evaluate()`, `error()`, `apply_correction()`
3. Register in constraint system

### Swapping the solver

```cpp
auto newton_solver = std::make_unique<NewtonRaphsonSolver>();
constraint_system.set_solver(std::move(newton_solver));
```

## License

MIT
