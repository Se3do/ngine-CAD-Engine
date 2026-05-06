# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Configure (debug)
cmake --preset debug

# Build
cmake --build build/debug

# Test
ctest --test-dir build/debug --output-on-failure

# Single test binary
./build/debug/tests/core/test_core

# Sanitizer build
cmake --preset sanitize && cmake --build build/sanitize

# Clang-tidy
cmake --preset clang-tidy && cmake --build build/clang-tidy

# Format check
find include src apps tests -name '*.hpp' -o -name '*.cpp' | xargs clang-format --dry-run --Werror
```

Docker alternatives:
```bash
docker compose run --rm test
docker compose run --rm tidy
docker compose run --rm format-check
```

## Architecture

Layered domain-oriented architecture. Dependencies flow downward only:

```
Interface (CLI, Document) → Commands, Constraints, Serialization, Spatial → Intersection → Core → Logging
```

Each module is a separate CMake library target (`ngine_core`, `ngine_intersection`, `ngine_constraints`, `ngine_commands`, `ngine_interface`, `ngine_serialization`, `ngine_spatial`, `ngine_logging`).

**Key patterns:**
- Geometry primitives are value types stored in `std::variant<Point, Line, Segment, Circle, Polygon>` (no inheritance)
- `IntersectionStrategy` — strategy pattern for swappable intersection algorithms
- `SolverStrategy` — pluggable constraint solver (sequential implemented, Newton-Raphson interface ready)
- `Command` — undo/redo via command pattern with `CommandHistory`
- `Document` — central entity store, owns CommandHistory + ConstraintSystem + IntersectionEngine

**Numerical conventions:**
- `Real` = `double`, tolerance = 1e-10 (absolute), 1e-8 (relative)
- Lines stored as normalized implicit form (ax+by+c=0, a²+b²=1)
- All floating-point comparisons use `nearly_equal()` / `nearly_zero()` from `core/types.hpp`

## Code Conventions

- C++20, Google-based clang-format (4-space indent, 100 col)
- Private members suffixed with `_`
- Factory methods (`from_points`, `from_coefficients`) over complex constructors
- `[[nodiscard]]` on all query methods
- Throw `std::invalid_argument` for precondition violations in constructors
- Headers in `include/ngine/<module>/`, sources in `src/<module>/`
