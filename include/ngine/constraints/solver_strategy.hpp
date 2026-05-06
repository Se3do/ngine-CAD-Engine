#pragma once

#include <ngine/constraints/constraint.hpp>
#include <ngine/core/types.hpp>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace ngine {

struct SolverConfig {
    std::size_t max_iterations{1000};
    Real convergence_threshold{1e-8};
    Real step_size{0.5};
};

struct SolverResult {
    bool converged{false};
    std::size_t iterations_used{0};
    Real final_error{0.0};
    std::vector<EntityId> unsatisfied_constraints;
};

class SolverStrategy {
public:
    virtual ~SolverStrategy() = default;

    [[nodiscard]] virtual SolverResult solve(
        std::vector<std::shared_ptr<Constraint>>& constraints,
        const SolverConfig& config) = 0;

    [[nodiscard]] virtual std::string name() const = 0;
};

}  // namespace ngine
