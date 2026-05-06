#pragma once

#include <ngine/constraints/solver_strategy.hpp>

namespace ngine {

class SequentialSolver final : public SolverStrategy {
public:
    [[nodiscard]] SolverResult solve(std::vector<std::shared_ptr<Constraint>>& constraints,
                                     const SolverConfig& config) override;

    [[nodiscard]] std::string name() const override { return "SequentialSolver"; }
};

}  // namespace ngine
