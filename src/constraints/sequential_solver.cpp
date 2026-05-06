#include <ngine/constraints/sequential_solver.hpp>

#include <cmath>

namespace ngine {

SolverResult SequentialSolver::solve(std::vector<std::shared_ptr<Constraint>>& constraints,
                                     const SolverConfig& config) {
    SolverResult result;
    result.converged = false;

    for (std::size_t iter = 0; iter < config.max_iterations; ++iter) {
        Real max_error = 0.0;

        for (auto& constraint : constraints) {
            Real err = constraint->error();
            if (err > config.convergence_threshold) {
                constraint->apply_correction(config.step_size);
            }
            max_error = std::max(max_error, constraint->error());
        }

        result.iterations_used = iter + 1;
        result.final_error = max_error;

        if (max_error <= config.convergence_threshold) {
            result.converged = true;
            return result;
        }
    }

    for (const auto& constraint : constraints) {
        if (constraint->error() > config.convergence_threshold) {
            result.unsatisfied_constraints.push_back(constraint->id());
        }
    }

    return result;
}

}  // namespace ngine
