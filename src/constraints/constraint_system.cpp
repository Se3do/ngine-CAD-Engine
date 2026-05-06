#include <ngine/constraints/constraint_system.hpp>
#include <ngine/constraints/sequential_solver.hpp>

#include <algorithm>

namespace ngine {

ConstraintSystem::ConstraintSystem(std::unique_ptr<SolverStrategy> solver)
    : solver_(std::move(solver)) {}

ConstraintSystem::ConstraintSystem() : solver_(std::make_unique<SequentialSolver>()) {}

void ConstraintSystem::add_constraint(std::shared_ptr<Constraint> constraint) {
    constraints_.push_back(std::move(constraint));
}

void ConstraintSystem::remove_constraint(EntityId id) {
    constraints_.erase(std::remove_if(constraints_.begin(), constraints_.end(),
                                      [id](const auto& c) { return c->id() == id; }),
                       constraints_.end());
}

void ConstraintSystem::clear() {
    constraints_.clear();
}

SolverResult ConstraintSystem::solve(const SolverConfig& config) {
    if (constraints_.empty()) {
        return {true, 0, 0.0, {}};
    }
    return solver_->solve(constraints_, config);
}

bool ConstraintSystem::is_consistent() const {
    for (const auto& constraint : constraints_) {
        if (constraint->evaluate() != ConstraintStatus::Satisfied) {
            return false;
        }
    }
    return true;
}

std::size_t ConstraintSystem::constraint_count() const noexcept {
    return constraints_.size();
}

void ConstraintSystem::set_solver(std::unique_ptr<SolverStrategy> solver) {
    solver_ = std::move(solver);
}

const std::vector<std::shared_ptr<Constraint>>& ConstraintSystem::constraints() const noexcept {
    return constraints_;
}

}  // namespace ngine
