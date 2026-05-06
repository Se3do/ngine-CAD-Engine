#pragma once

#include <ngine/constraints/constraint.hpp>
#include <ngine/constraints/solver_strategy.hpp>

#include <memory>
#include <vector>

namespace ngine {

class ConstraintSystem {
public:
    explicit ConstraintSystem(std::unique_ptr<SolverStrategy> solver);
    ConstraintSystem();

    void add_constraint(std::shared_ptr<Constraint> constraint);
    void remove_constraint(EntityId id);
    void clear();

    [[nodiscard]] SolverResult solve(const SolverConfig& config = {});
    [[nodiscard]] bool is_consistent() const;
    [[nodiscard]] std::size_t constraint_count() const noexcept;

    void set_solver(std::unique_ptr<SolverStrategy> solver);
    [[nodiscard]] const std::vector<std::shared_ptr<Constraint>>& constraints() const noexcept;

private:
    std::unique_ptr<SolverStrategy> solver_;
    std::vector<std::shared_ptr<Constraint>> constraints_;
};

}  // namespace ngine
