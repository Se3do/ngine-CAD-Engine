# Architecture

## Overview

ngine is a 2D geometry engine implementing core CAD subsystem concepts: geometric modeling, intersection detection, constraint solving, and command-based editing with undo/redo.

## Module Dependency Graph

```
                    ┌──────────┐
                    │   apps   │
                    └────┬─────┘
                         │
                    ┌────▼─────┐
                    │interface │ (Document, CLI, Parser)
                    └────┬─────┘
           ┌─────────┬──┴──┬──────────┬────────────┐
           │         │     │          │            │
    ┌──────▼──┐ ┌───▼───┐ │  ┌──────▼─────┐ ┌───▼────┐
    │commands │ │constr. │ │  │serialization│ │spatial │
    └────┬────┘ └───┬───┘ │  └──────┬─────┘ └───┬────┘
         │          │     │         │            │
         └──────────┴─────┼─────────┴────────────┘
                          │
                   ┌──────▼──────┐
                   │intersection │
                   └──────┬──────┘
                          │
                     ┌────▼───┐
                     │  core  │
                     └────┬───┘
                          │
                    ┌─────▼────┐
                    │ logging  │
                    └──────────┘
```

## Key Design Decisions

### 1. Value-Type Geometry (no polymorphism)

Geometry primitives (Point, Line, Segment, Circle, Polygon) are value types. Entity storage uses `std::variant` instead of a base class hierarchy. This gives:
- No heap allocation per entity
- No virtual dispatch for basic operations
- Cache-friendly storage
- Natural copy/move semantics

### 2. Normalized Implicit Line Representation

Lines are stored as ax + by + c = 0 where a² + b² = 1. Benefits:
- `signed_distance(point)` = direct evaluation (no division)
- Parallel/perpendicular checks = single cross/dot product
- Numerically stable intersection computation
- No special-casing for vertical lines

### 3. Strategy Pattern for Algorithms

Both intersection computation and constraint solving use the Strategy pattern:
- `IntersectionStrategy` — allows swapping algorithms (e.g., robust vs. fast)
- `SolverStrategy` — sequential solver now, Newton-Raphson later

This satisfies the Open/Closed Principle: new algorithms don't modify existing code.

### 4. Sequential Constraint Solver (Gauss-Seidel)

The initial solver iterates constraints one-by-one, applying corrections each step. Trade-offs:
- Simple to implement and debug
- Converges for well-constrained systems
- May oscillate or fail for tightly coupled constraints
- Newton-Raphson (Jacobian-based) would handle coupled systems better

The `SolverStrategy` interface allows upgrading without touching constraint code.

### 5. Command Pattern with Bounded History

Commands capture state needed for undo. The history uses a bounded deque (default 100) to prevent unbounded memory growth. Executing a new command clears the redo stack.

### 6. Document as Aggregate Root

`Document` owns entities, command history, constraint system, and intersection engine. It's the single entry point for all mutations, making it easy to serialize and reason about state.

## Numerical Stability

- All comparisons use epsilon-based tolerance (1e-10 absolute)
- Line normalization prevents coefficient magnitude issues
- Intersection algorithms handle degenerate cases (parallel, tangent, coincident)
- Constraint solver uses configurable convergence threshold
